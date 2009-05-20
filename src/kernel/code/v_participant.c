/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2009 PrismTech 
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */
#include "v__participant.h"
#include "v__participantQos.h"
#include "v__kernel.h"
#include "v__leaseManager.h"
#include "v__publisher.h"
#include "v__writer.h"
#include "v__subscriber.h"
#include "v_dataReader.h"
#include "v__reader.h"
#include "v__observer.h"
#include "v__builtin.h"
#include "v__lease.h"
#include "v__waitset.h"
#include "v__observable.h"

#include "v_entity.h"
#include "v_event.h"
#include "v_public.h"
#include "v_proxy.h"

#include "v_topic.h"
#include "v_subscriberQos.h"
#include "v_readerQos.h"
#include "v_dataReader.h"
#include "v_time.h"
#include "v_groupSet.h"
#include "v_group.h"
#include "v_statistics.h"
#include "v_event.h"

#include "os_report.h"
#include "q_expr.h"

#define V_BUILTINSUBSCRIBER_NAME "__BUILTIN SUBSCRIBER__"

v_participant
v_participantNew(
    v_kernel kernel,
    const c_char *name,
    v_qos qos,
    v_statistics s,
    c_bool enable)
{
    v_participant p;
    v_participantQos q;

    assert(C_TYPECHECK(kernel,v_kernel));
    /* Do not use C_TYPECHECK on qos parameter,
     * since it might be allocated on heap! */

    /* do no use cast method on qos parameter,
     * it is most likely allocated on heap! */
    q = v_participantQosNew(kernel, (v_participantQos)qos);
    if (q == NULL) {
        OS_REPORT(OS_ERROR, "v_participantNew", 0,
                  "Participant not created: inconsistent qos");
        p = NULL;
    } else {
        p = v_participant(v_objectNew(kernel,K_PARTICIPANT));
        v_participantInit(p,name,q,s,enable);
        c_free(q);
        v_addParticipant(kernel,p);
    }

    return p;
}

void
v_participantInit(
    v_participant p,
    const c_char *name,
    v_participantQos qos,
    v_statistics s,
    c_bool enable)
{
    v_kernel kernel;
    c_base base;
    v_message builtinMsg;
    c_type writerProxyType;

    assert(C_TYPECHECK(p,v_participant));
    /* Do not use C_TYPECHECK on qos parameter,
     * since it might be allocated on heap! */

    kernel = v_objectKernel(p);
    base = c_getBase(p);
    v_observerInit(v_observer(p),name,s,enable);

    p->entities = c_setNew(c_resolve(base,"kernelModule::v_entity"));
    p->qos = c_keep(qos);
    /* Currently default LIVELINESS policy is used: kind=AUTOMATIC,
     * duration=INFINITE This setting implies no lease registration.
    */
    p->lease = NULL;
    p->leaseManager = v_leaseManagerNew(kernel);
    p->resendQuit = FALSE;
    c_mutexInit(&p->resendMutex, SHARED_MUTEX);
    c_condInit(&p->resendCond, &p->resendMutex, SHARED_COND);
    writerProxyType = v_proxy_t(p);
    p->resendWriters = c_tableNew(writerProxyType, "source.index,source.serial");
    c_free(writerProxyType);

    p->builtinSubscriber = NULL;
    if (!v_observableAddObserver(v_observable(kernel),v_observer(p), NULL)) {
        if (name != NULL) {
            OS_REPORT_1(OS_WARNING,"Kernel Participant",0,
                        "%s: Cannot observe Kernel events",name);
        } else {
            OS_REPORT(OS_WARNING,"Kernel Participant",0,
                      "Cannot observe Kernel events");
        }
    }

    v_observerSetEventMask(v_observer(p), V_EVENT_NEW_GROUP);
    c_lockInit(&p->lock,SHARED_LOCK);
    c_mutexInit(&p->builtinLock,SHARED_MUTEX);

    /* Here the Builtin Topic of the participant is published.
     * This call mabe a noop in case builtin is disabled on kernel level.
     */
    builtinMsg = v_builtinCreateParticipantInfo(kernel->builtin,p);
    v_writeBuiltinTopic(kernel, V_PARTICIPANTINFO_ID, builtinMsg);
    c_free(builtinMsg);
}

void
v_participantFree(
    v_participant p)
{
    v_message builtinMsg;
    v_participant found;
    v_entity e;
    v_kernel kernel;

    if (p != NULL) {
        assert(C_TYPECHECK(p,v_participant));

        kernel = v_objectKernel(p);

        builtinMsg = v_builtinCreateParticipantInfo(kernel->builtin,p);
        v_writeDisposeBuiltinTopic(kernel, V_PARTICIPANTINFO_ID, builtinMsg);
        c_free(builtinMsg);

        builtinMsg = v_builtinCreateParticipantInfo(kernel->builtin,p);
        v_unregisterBuiltinTopic(kernel, V_PARTICIPANTINFO_ID, builtinMsg);
        c_free(builtinMsg);

        if (p->builtinSubscriber) {
            v_subscriberFree(p->builtinSubscriber);
            p->builtinSubscriber = NULL;
        }
        while ((e = c_take(p->entities)) != NULL) {
            switch (v_objectKind(e)) {
            case K_PUBLISHER:
                v_publisherFree(v_publisher(e));
            break;
            case K_SUBSCRIBER:
                v_subscriberFree(v_subscriber(e));
            break;
            case K_WAITSET:
                v_waitsetFree(v_waitset(e));
            break;
            default:
                OS_REPORT_1(OS_WARNING,"Kernel Participant",0,
                            "Illegal contained object (%s)",
                            v_participantName(p));
            break;
            }
            c_free(e); /* deref o since c_take will not */
        }
        if (!v_observableRemoveObserver(v_observable(kernel),v_observer(p))) {
            if (v_participantName(p) != NULL) {
                OS_REPORT_1(OS_WARNING,"Kernel Participant",0,
                            "%s: Cannot observe Kernel events",
                            v_participantName(p));
            } else {
                OS_REPORT(OS_WARNING,"Kernel Participant",0,
                          "Cannot observe Kernel events");
            }
        }

        found = v_removeParticipant(kernel,p);
        assert(found == p);
        c_free(found);

        v_observerFree(v_observer(p));
    }
}

void
v_participantDeinit(
    v_participant p)
{
    v_kernel kernel;

    assert(C_TYPECHECK(p,v_participant));

    kernel = v_objectKernel(p);

    v_leaseManagerDeregister(kernel->livelinessLM, p->lease);
    c_free(p->lease);
    p->lease = NULL;
    v_leaseManagerFree(p->leaseManager);
    p->leaseManager = NULL;

    v_observerDeinit(v_observer(p));
}

void
v_participantAdd(
    v_participant p,
    v_entity e)
{
    v_entity found;

    assert(p != NULL);
    assert(C_TYPECHECK(p,v_participant));
    assert(e != NULL);
    assert(C_TYPECHECK(e,v_entity));

    c_lockWrite(&p->lock);
    found = c_setInsert(p->entities,e);
    c_lockUnlock(&p->lock);
    assert(found == e);
}

void
v_participantRemove(
    v_participant p,
    v_entity e)
{
    v_entity found;

    assert(p != NULL);
    assert(C_TYPECHECK(p,v_participant));
    assert(e != NULL);
    assert(C_TYPECHECK(e,v_entity));

    c_lockWrite(&p->lock);
    found = c_remove(p->entities,e,NULL,NULL);
    c_lockUnlock(&p->lock);
    c_free(found);
}


static c_bool
autoConnect(
    c_object o,
    c_voidp arg)
{
    switch (v_objectKind(o)) {
    case K_PUBLISHER:
        v_publisherNotify(v_publisher(o), (v_event)arg);
    break;
    case K_SUBSCRIBER:
        v_subscriberNotify(v_subscriber(o), (v_event)arg);
    break;
    default:
        /* Other entities do not connect */
    break;
    }
    return TRUE;
}

static c_bool
assertLivelinessPublisher(
    c_object o,
    c_voidp arg)
{
    v_entity e = v_entity(o);

    if (v_objectKind(e) == K_PUBLISHER) {
        v_publisherNotify(v_publisher(e), (v_event)arg);
    }
    return TRUE;
}

void
v_participantDeleteHistoricalData(
    v_participant participant,
    const c_char* partitionExpr,
    const c_char* topicExpr)
{
    c_iter matchingGroups;
    v_group group;
    c_time t;
    c_value params[2];
    C_STRUCT(v_event) event;
    C_STRUCT(v_historyDeleteEventData) hde;

    assert(participant != NULL);
    assert(C_TYPECHECK(participant, v_participant));
    assert(partitionExpr);
    assert(topicExpr);

    if(partitionExpr && topicExpr){
        params[0]  = c_stringValue((c_string)partitionExpr);
        params[1]  = c_stringValue((c_string)topicExpr);

        c_lockRead(&participant->lock);
        t = v_timeGet();
        matchingGroups = v_groupSetSelect(
                                v_objectKernel(participant)->groupSet,
                                "partition.name like %0 AND topic.name like %1",
                                params);
        c_lockUnlock(&participant->lock);

        group = v_group(c_iterTakeFirst(matchingGroups));
        while(group){
            v_groupDeleteHistoricalData(group, t);
            c_free(group);
            group = v_group(c_iterTakeFirst(matchingGroups));
        }
        c_iterFree(matchingGroups);


        hde.partitionExpression = (c_char *)partitionExpr;
        hde.topicExpression = (c_char *)topicExpr;
        hde.deleteTime = t;
        event.kind = V_EVENT_HISTORY_DELETE;
        event.source = v_publicHandle(v_public(participant));
        event.userData = &hde;
        v_observableNotify(v_observable(v_objectKernel(participant)),&event);
    }
    return;
}

void
v_participantNotify(
    v_participant p,
    v_event event,
    c_voidp userData)
{
    assert(p != NULL);
    assert(C_TYPECHECK(p,v_participant));

    if (event) {
        switch (event->kind) {
        case V_EVENT_NEW_GROUP:
            c_lockWrite(&p->lock);
            c_walk(p->entities, autoConnect, event);
            c_lockUnlock(&p->lock);
        break;
        case V_EVENT_SERVICESTATE_CHANGED:
        break;
        case V_EVENT_DATA_AVAILABLE:
        break;
        case V_EVENT_LIVELINESS_ASSERT:
            /* assert all writers */
            c_lockWrite(&p->lock);
            c_walk(p->entities, assertLivelinessPublisher, event);
            c_lockUnlock(&p->lock);
        break;
        case V_EVENT_HISTORY_DELETE:
            /*Do nothing here.*/
        break;
        case V_EVENT_HISTORY_REQUEST:
            /*Do nothing here.*/
        break;
        default:
            OS_REPORT_1(OS_WARNING,"Kernel Participant",0,
                        "Notify encountered unknown event kind (%d)",event->kind);
        break;
        }
    }
}

void
v_participantAssertLiveliness(
    v_participant p)
{
    C_STRUCT(v_event) event;

    assert(p != NULL);
    assert(C_TYPECHECK(p,v_participant));

    event.kind = V_EVENT_LIVELINESS_ASSERT;
    event.source = v_publicHandle(v_public(p));
    event.userData = NULL;
    /* Walk over all entities and assert liveliness on all writers */
    c_lockWrite(&p->lock);
    c_walk(p->entities, assertLivelinessPublisher, &event);
    c_lockUnlock(&p->lock);
}

void
v_participantRenewLease(
    v_participant p,
    v_duration leasePeriod)
{
    assert(p != NULL);
    assert(C_TYPECHECK(p, v_participant));

    c_lockWrite(&p->lock);
    v_leaseRenew(p->lease, leasePeriod);
    c_lockUnlock(&p->lock);
}

v_subscriber
v_participantGetBuiltinSubscriber(
    v_participant p)
{
    v_subscriberQos sQos;
    v_readerQos rQos;
    v_kernel kernel;
    c_bool create_builtin_readers = FALSE;

    assert(p != NULL);
    assert(C_TYPECHECK(p, v_participant));

    c_mutexLock(&p->builtinLock);
    if (p->builtinSubscriber == NULL) {
        kernel = v_objectKernel(p);
        sQos = v_subscriberQosNew(kernel, NULL);
        sQos->presentation.access_scope = V_PRESENTATION_TOPIC;
        c_free(sQos->partition);
        sQos->partition = c_stringNew(c_getBase(c_object(kernel)),
                                      V_BUILTIN_PARTITION);
        sQos->entityFactory.autoenable_created_entities = TRUE;

        p->builtinSubscriber = v_subscriberNew(p, V_BUILTINSUBSCRIBER_NAME,
                                               sQos, TRUE);
        v_subscriberQosFree(sQos);

        create_builtin_readers = TRUE;
        c_mutexUnlock(&p->builtinLock);

        assert(p->builtinSubscriber != NULL);

        rQos = v_readerQosNew(kernel, NULL);
        rQos->durability.kind = V_DURABILITY_TRANSIENT;
        rQos->reliability.kind = V_RELIABILITY_RELIABLE;
        rQos->history.kind = V_HISTORY_KEEPLAST;
        rQos->history.depth = 1;

#define _CREATE_READER_(topicName) {\
            q_expr expr; \
            v_dataReader dr; \
            expr = q_parse("select * from " topicName);\
            dr = v_dataReaderNew(p->builtinSubscriber, topicName "Reader", \
                                   expr, NULL, rQos, TRUE);\
            c_free(dr); \
            q_dispose(expr); \
        }
        _CREATE_READER_(V_PARTICIPANTINFO_NAME)
        _CREATE_READER_(V_TOPICINFO_NAME)
        _CREATE_READER_(V_PUBLICATIONINFO_NAME)
        _CREATE_READER_(V_SUBSCRIPTIONINFO_NAME)
#undef _CREATE_READER_
        v_readerQosFree(rQos);
    } else {
        c_mutexUnlock(&p->builtinLock);
    }

    return c_keep(p->builtinSubscriber);
}

v_result
v_participantSetQos(
    v_participant p,
    v_participantQos qos)
{
    v_message builtinMsg;
    v_kernel kernel;
    v_qosChangeMask cm;
    v_result result;

    assert(C_TYPECHECK(p,v_participant));
    /* Do not use C_TYPECHECK on qos parameter, since it might be allocated on heap! */

    kernel = v_objectKernel(p);
    c_lockWrite(&p->lock);
    result = v_participantQosSet(p->qos, qos, &cm);

    if ((result == V_RESULT_OK) && (cm != 0)) {
        builtinMsg = v_builtinCreateParticipantInfo(kernel->builtin,p);
        c_lockUnlock(&p->lock);
        v_writeBuiltinTopic(kernel, V_PARTICIPANTINFO_ID, builtinMsg);
        c_free(builtinMsg);
    } else {
        c_lockUnlock(&p->lock);
    }

    return result;
}

v_leaseManager
v_participantGetLeaseManager(
    v_participant p)
{
    assert(C_TYPECHECK(p,v_participant));

    return c_keep(p->leaseManager);
}

#define RESEND_SECS      (0U)
#define RESEND_NANOSECS  (2000000U) /* 2 ms */
void
v_participantResendManagerMain(
    v_participant p)
{
    c_iter writerProxies;
    v_proxy wp;
    v_writer w;
    v_handleResult r;
    c_time waitTime = { RESEND_SECS, RESEND_NANOSECS };

    assert(C_TYPECHECK(p,v_participant));

    c_mutexLock(&p->resendMutex);
    while (!p->resendQuit) {

        if (c_count(p->resendWriters) == 0) {
            c_condWait(&p->resendCond, &p->resendMutex);
        } else {
            c_condTimedWait(&p->resendCond, &p->resendMutex, waitTime);
        }

        if (!p->resendQuit) {
            writerProxies = c_select(p->resendWriters, 0);
            c_mutexUnlock(&p->resendMutex);
            wp = v_proxy(c_iterTakeFirst(writerProxies));
            while (wp != NULL) {
                r = v_handleClaim(wp->source,(v_object *)&w);
                if (r == V_HANDLE_OK) {
                    assert(C_TYPECHECK(w,v_writer));
                    v_writerResend(w);
                    v_handleRelease(wp->source);
                }
                c_free(wp);
                wp = v_proxy(c_iterTakeFirst(writerProxies));
            }
            c_iterFree(writerProxies);
            c_mutexLock(&p->resendMutex);
        } /* already quiting */
    }
    c_mutexUnlock(&p->resendMutex);
}

void
v_participantResendManagerQuit(
    v_participant p)
{
    assert(C_TYPECHECK(p,v_participant));

    c_mutexLock(&p->resendMutex);
    p->resendQuit = TRUE;
    c_condBroadcast(&p->resendCond);
    c_mutexUnlock(&p->resendMutex);
}

void
v_participantResendManagerAddWriter(
    v_participant p,
    v_writer w)
{
    v_proxy wp, found;
    assert(C_TYPECHECK(p,v_participant));

    wp = v_proxyNew(v_objectKernel(w), v_publicHandle(v_public(w)), NULL);

    c_mutexLock(&p->resendMutex);
    found = c_insert(p->resendWriters, wp);
    assert((found->source.index == wp->source.index) &&
            (found->source.serial == wp->source.serial));
    c_condBroadcast(&p->resendCond);
    c_mutexUnlock(&p->resendMutex);

    c_free(wp);
}

void
v_participantResendManagerRemoveWriter(
    v_participant p,
    v_writer w)
{
    C_STRUCT(v_proxy) wp;
    v_proxy found;

    wp.source = v_publicHandle(v_public(w));
    wp.userData = NULL;
    c_mutexLock(&p->resendMutex);
    found = c_remove(p->resendWriters, &wp, NULL, NULL);
    c_free(found); /* remove local reference transferred from collection */
    c_mutexUnlock(&p->resendMutex);
}