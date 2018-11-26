/*
 *                         Vortex OpenSplice
 *
 *   This software and documentation are Copyright 2006 to TO_YEAR ADLINK
 *   Technology Limited, its affiliated companies and licensors. All rights
 *   reserved.
 *
 *   Licensed under the ADLINK Software License Agreement Rev 2.7 2nd October
 *   2014 (the "License"); you may not use this file except in compliance with
 *   the License.
 *   You may obtain a copy of the License at:
 *                      $OSPL_HOME/LICENSE
 *
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */

/** \file os/qnx/code/os_rwlock.c
 *  \brief QNX multiple reader writer lock
 *
 * Implements multiple reader writer lock for QNX
 * by including the common mutex based implementation
 */

#include "../common/code/os_rwlock_by_mutex.c"
#include "../common/code/os_rwlock_attr.c"
