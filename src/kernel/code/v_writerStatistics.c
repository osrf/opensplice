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
#include "v__statistics.h"
#include "v__statisticsInterface.h"
#include "v_writerStatistics.h"

static c_type writerStatisticsType = NULL;

v_writerStatistics 
v_writerStatisticsNew(
    v_kernel k)
{
    v_writerStatistics ws;

    assert(k != NULL);
    assert(C_TYPECHECK(k, v_kernel));

    if (writerStatisticsType == NULL) {
        writerStatisticsType = c_resolve(c_getBase(k), "kernelModule::v_writerStatistics");
    }
    ws = v_writerStatistics(v_new(k, writerStatisticsType));
    v_writerStatisticsInit(ws);
    return ws;
}

void
v_writerStatisticsInit(
    v_writerStatistics ws)
{
    assert(ws != NULL);
    assert(C_TYPECHECK(ws, v_writerStatistics));

    v_statisticsInit(v_statistics(ws));
    ws->numberOfWrites = 0;
    ws->numberOfDisposes = 0;
    ws->numberOfRegisters = 0;
    ws->numberOfImplicitRegisters = 0;
    ws->numberOfUnregisters = 0;
    ws->numberOfBlockedWrites = 0;
    ws->numberOfRetries = 0;

    ws->numberOfAliveInstances = 0;
    ws->numberOfDisposedInstances = 0;
    ws->numberOfUnregisteredInstances = 0;
    ws->numberOfSamples = 0;
    v_maxValueInit(&ws->maxNumberOfSamplesPerInstance);    
}

void
v_writerStatisticsDeinit(
    v_writerStatistics ws)
{
    assert(ws!=NULL);
    assert(C_TYPECHECK(ws, v_writerStatistics));
}

c_bool
v_writerStatisticsReset(
    v_writerStatistics ws,
    const c_char* fieldName)
{
    c_bool result;

    assert(ws!=NULL);
    assert(C_TYPECHECK(ws, v_writerStatistics));

    result = FALSE;

    if (fieldName != NULL) {
        result = v_statisticsResetField(v_statistics(ws), fieldName);
    } else {
        v_statisticsULongResetInternal(v_writer, numberOfWrites, ws);
        v_statisticsULongResetInternal(v_writer, numberOfDisposes, ws);
        v_statisticsULongResetInternal(v_writer, numberOfRegisters, ws);
        v_statisticsULongResetInternal(v_writer, numberOfImplicitRegisters, ws);
        v_statisticsULongResetInternal(v_writer, numberOfUnregisters, ws);
        v_statisticsULongResetInternal(v_writer, numberOfBlockedWrites, ws);
        v_statisticsULongResetInternal(v_writer, numberOfRetries, ws);

        v_statisticsULongResetInternal(v_writer, numberOfAliveInstances, ws);
        v_statisticsULongResetInternal(v_writer, numberOfDisposedInstances, ws);
        v_statisticsULongResetInternal(v_writer, numberOfUnregisteredInstances, ws);
        v_statisticsULongResetInternal(v_writer, numberOfSamples, ws);
        v_maxValueReset(&ws->maxNumberOfSamplesPerInstance);

        result = TRUE;
    }
    return result;
}

void
v_writerStatisticsFree(
    v_writerStatistics ws)
{
    assert(ws != NULL);
    assert(C_TYPECHECK(ws, v_writerStatistics));

    v_writerStatisticsDeinit(ws);
    c_free(ws);
}
