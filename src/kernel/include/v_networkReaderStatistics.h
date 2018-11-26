/*
 *                         Vortex OpenSplice
 *
 *   This software and documentation are Copyright 2006 to TO_YEAR ADLINK
 *   Technology Limited, its affiliated companies and licensors. All rights
 *   reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
#ifndef V_NETWORKREADERSTATISTICS_H
#define V_NETWORKREADERSTATISTICS_H

#include "v_kernel.h"
#include "os_if.h"

#ifdef OSPL_BUILD_CORE
#define OS_API OS_API_EXPORT
#else
#define OS_API OS_API_IMPORT
#endif
/* !!!!!!!!NOTE From here no more includes are allowed!!!!!!! */

/**
 * \brief The <code>v_networkReaderStatistics</code> cast method.
 *
 * This method casts an object to a <code>v_networkReaderStatistics</code> object.
 * Before the cast is performed, if compiled with the NDEBUG flag not set,
 * the type of the object is checked to be <code>v_networkReaderStatistics</code> or
 * one of its subclasses.
 */
#define v_networkReaderStatistics(_this) (C_CAST(_this,v_networkReaderStatistics))

OS_API v_networkReaderStatistics
v_networkReaderStatisticsNew(
    v_kernel k);

OS_API void
v_networkReaderStatisticsInit(
    v_networkReaderStatistics _this);
    
OS_API void
v_networkReaderStatisticsDeinit(
    v_networkReaderStatistics _this);
    
OS_API void
v_networkReaderStatisticsFree(
    v_networkReaderStatistics _this);

#undef OS_API

#endif
