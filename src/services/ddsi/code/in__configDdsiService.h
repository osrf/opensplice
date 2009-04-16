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
/* Prevent failure due to multiple inclusion of this file. */
#ifndef IN_CONFIG_DDSI_SERVICE_H
#define IN_CONFIG_DDSI_SERVICE_H

/* OS abstraction includes. */
#include "os_defs.h"
#include "os_classbase.h"
#include "os_stdlib.h"
#include "os_heap.h"

/* DDSi configuration includes */
#include "in__configDiscoveryChannel.h"
#include "in__configDataChannel.h"
#include "in__configTracing.h"
#include "in__configPartitioning.h"
#include "in__configDebug.h"
#include "in__configTypes.h"

/* Collection includes */
#include "Coll_List.h"

/**
 * Allow usage of this C code from C++ code.
 */
#if defined (__cplusplus)
extern "C" {
#endif



/**
 * Macro that allows the implementation of type checking when casting an
 * object. The signature of the 'casting macro' must look like this:
 */
#define in_configDdsiService(_this) ((in_configDdsiService)_this)

in_configDdsiService
in_configDdsiServiceNew(
    os_char* name);

/* takes ownership of the discoverychannel */
void
in_configDdsiServiceSetDiscoveryChannelConfig(
    in_configDdsiService _this,
    in_configDiscoveryChannel discoveryChannel);

void
in_configDdsiServiceAddDataChannelConfig(
    in_configDdsiService _this,
    in_configDataChannel channel);

void
in_configDdsiServiceSetNetworkId(
    in_configDdsiService _this,
    os_char* networkId);

os_char*
in_configDdsiServiceGetName(
    in_configDdsiService _this);

os_char*
in_configDdsiServiceGetPathName(
    in_configDdsiService _this);

os_char*
in_configDdsiServiceGetInterfaceId(
    in_configDdsiService _this);

/* List contains 'in_configDataChannel' object */
Coll_List*
in_configDdsiServiceGetChannels(
    in_configDdsiService _this);

/* return first object matching the name */
in_configDataChannel
in_configDdsiServiceGetChannel(
    in_configDdsiService _this,
    const os_char *name);

in_configDiscoveryChannel
in_configDdsiServiceGetDiscoveryChannel(
    in_configDdsiService _this);

in_configTracing
in_configDdsiServiceGetTracing(
    in_configDdsiService _this);

in_configPartitioning
in_configDdsiServiceGetPartitioning(
    in_configDdsiService _this);

in_configDebug
in_configDdsiServiceGetDebugging(
    in_configDdsiService _this);

void
in_configDdsiServiceSetPartitioning(
    in_configDdsiService _this,
    in_configPartitioning partitioning);

/* Close the brace that allows the usage of this code in C++. */
#if defined (__cplusplus)
}
#endif

#endif /* IN_CONFIG_DDSI_SERVICE_H */
