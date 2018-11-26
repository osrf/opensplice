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

#ifndef OS_DARWIN_COND_H
#define OS_DARWIN_COND_H

#if defined (__cplusplus)
extern "C" {
#endif

#include <pthread.h>

  typedef struct os_os_cond {
#ifdef OSPL_STRICT_MEM
    uint64_t signature; /* Used to identify initialized cond when memory is
			   freed - keep this first in the structure so its
			   so its address is the same as the os_cond */
#endif
    pthread_cond_t cond;
  } os_os_cond;

#if defined (__cplusplus)
}
#endif

#endif /* OS_DARWIN_COND_H */
