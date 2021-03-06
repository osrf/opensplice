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
#ifndef OSPL_DDS_SUB_TCOHERENTACCESS_HPP_
#define OSPL_DDS_SUB_TCOHERENTACCESS_HPP_

/**
 * @file
 */

/*
 * OMG PSM class declaration
 */
#include <spec/dds/sub/TCoherentAccess.hpp>

// Implementation
namespace dds
{
namespace sub
{

template <typename D>
TCoherentAccess<D>::~TCoherentAccess(void)
{
    this->delegate().end();
}

template <typename D>
TCoherentAccess<D>::TCoherentAccess(const dds::sub::Subscriber& sub) : dds::core::Value<D>(sub) { }

template <typename D>
void TCoherentAccess<D>::end()
{
    this->delegate().end();
}
}
}
// End of implementation

#endif /* OSPL_DDS_SUB_TCOHERENTACCESS_HPP_ */
