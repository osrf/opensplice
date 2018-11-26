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
#ifndef OS_VXWORKS_STDLIB_H
#define OS_VXWORKS_STDLIB_H

#if defined (OSPL_VXWORKS653)
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN    64
#endif
#endif

#include <envLib.h>

#include <../posix/include/os_os_stdlib.h>

#if defined (__cplusplus)
extern "C" {
#endif

/* The function S_ISLNK does not exist on Vxworks, so redefine */
#ifdef OS_ISLNK
#undef OS_ISLNK
#define OS_ISLNK(x) (0)
#endif

void os_stdlibInitialize();


#if defined (__cplusplus)
}
#endif

#endif /* OS_VXWORKS_STDLIB_H */
