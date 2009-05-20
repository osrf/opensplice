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

#ifndef D_CONFIGURATION_H
#define D_CONFIGURATION_H

#include "d__types.h"
#include "d_object.h"
#include "u_user.h"
#include "v_kernel.h"

#if defined (__cplusplus)
extern "C" {
#endif

C_STRUCT(d_configuration){
    C_EXTENDS(d_object);
    v_duration    livelinessExpiryTime;
    c_float       livelinessExpiry;
    os_time       livelinessUpdateInterval;
    os_threadAttr livelinessScheduling;      /* Not implemented yet.*/
    os_time       heartbeatUpdateInterval;
    os_time       heartbeatExpiryTime;
    c_float       heartbeatExpiry;
    os_threadAttr heartbeatScheduling;
    os_time       timingInitialWaitPeriod;
    c_ulong       networkMaxWaitCount;
    os_time       networkMaxWaitTime;
    d_table       networkServiceNames;
    c_iter        services;
    os_time       networkSampleResendTimeRange;
    c_char*       publisherName;
    c_char*       subscriberName;
    c_char*       partitionName;
    FILE*         tracingOutputFile;
    c_char*       tracingOutputFileName;
    c_bool        tracingSynchronous;
    c_bool        tracingTimestamps;
    c_bool        tracingRelativeTimestamps;
    d_level       tracingVerbosityLevel;
    c_char*       persistentStoreDirectory;
    d_storeType   persistentStoreMode;
    os_time       persistentStoreSleepTime;
    os_time       persistentStoreSessionTime;
    c_ulong       persistentQueueSize;
    os_threadAttr persistentScheduling;
    c_iter        nameSpaces;
    os_time       startTime;
    c_ulong       persistentUpdateInterval;

    v_duration    latencyBudget;
    c_long        transportPriority;
    v_duration    heartbeatLatencyBudget;
    c_long        heartbeatTransportPriority;
    v_duration    alignerLatencyBudget;
    c_long        alignerTransportPriority;
    os_threadAttr alignerScheduling;
    os_threadAttr aligneeScheduling;
    os_time       initialRequestCombinePeriod;
    os_time       operationalRequestCombinePeriod;
    c_bool        timeAlignment;
};

#define d_configuration(d) ((d_configuration)(d))

/**
 * Allocates a new configuration.
 * The configuration is read from the user layer service of the supplied
 * d_durability object.
 *
 * @param service The object to resolve the user layer service from. This
 *                contains the configuration that was read when creating it.
 * @param serviceName The name of the service. This is used to locate the
 *                    correct configuration entity in the configuration file.
 *                    The 'name' attribute of the durability service in the
 *                    configuration must match this parameter.
 * @return The read configuration. The configuration items that were not in
 *         the configuration file have the default value.
 */
d_configuration d_configurationNew                  (d_durability service,
                                                     const c_char* serviceName,
                                                     c_long domainId);

/**
 * Frees the supplied configuration.
 *
 * @param configuration The configuration to free.
 */
void            d_configurationFree                 (d_configuration configuration);

/**
 * Determines whether the supplied partition-topic combination is in the
 * alignee namespace for non-volatile data.
 *
 * @param configuration The configuration that contains the namespaces.
 * @param partition The partition to check.
 * @param topic The topic to check.
 * @param kind The durability kind.
 * @return TRUE if it is in the namespace, FALSE otherwise.
 */
c_bool          d_configurationGroupInAligneeNS         (d_configuration configuration,
                                                         d_partition partition,
                                                         d_topic topic,
                                                         d_durabilityKind kind);


/**
 * Determines whether the supplied partition-topic combination is in the
 * alignee namespace for non-volatile data and the
 * alignmentKind != D_ALIGNMENT_ON_REQUEST.
 *
 * @param configuration The configuration that contains the namespaces.
 * @param partition The partition to check.
 * @param topic The topic to check.
 * @param kind The durability kind.
 * @return TRUE if it is in the namespace, FALSE otherwise.
 */
c_bool          d_configurationGroupInActiveAligneeNS   (d_configuration configuration,
                                                         d_partition partition,
                                                         d_topic topic,
                                                         d_durabilityKind kind);

/**
 * Determines whether the supplied partition-topic combination is in the
 * aligner namespace for persistent data.
 *
 * @param configuration The configuration that contains the namespaces.
 * @param partition The partition to check.
 * @param topic The topic to check.
 * @return TRUE if it is in the namespace, FALSE otherwise.
 */
c_bool          d_configurationGroupInAlignerNS         (d_configuration configuration,
                                                         d_partition partition,
                                                         d_topic topic,
                                                         d_durabilityKind kind);

c_bool          d_configurationGroupInInitialAligneeNS  (d_configuration configuration,
                                                         d_partition partition,
                                                         d_topic topic,
                                                         d_durabilityKind kind);

c_bool          d_configurationInNameSpace              (d_nameSpace ns,
                                                         d_partition partition,
                                                         d_topic topic,
                                                         d_durabilityKind kind,
                                                         c_bool aligner);

d_nameSpace     d_configurationGetNameSpaceForGroup     (d_configuration configuration,
                                                         d_partition partition,
                                                         d_topic topic,
                                                         d_durabilityKind kind);

#if defined (__cplusplus)
}
#endif

#endif /* D_CONFIGURATION_H */