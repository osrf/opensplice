#ifndef SPEC_DDS_STREAMS_SUB_STREAMSAMPLE_HPP_
#define SPEC_DDS_STREAMS_SUB_STREAMSAMPLE_HPP_
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

/**
 * @file
 */

#include <dds/streams/sub/detail/StreamSample.hpp>

namespace dds
{
namespace streams
{
namespace sub
{
    template <typename T, template <typename Q> class DELEGATE = dds::streams::sub::detail::StreamSample >
class StreamSample;
}
}
}

#include <dds/streams/sub/TStreamSample.hpp>

#endif /* SPEC_DDS_STREAMS_SUB_STREAMSAMPLE_HPP_ */
