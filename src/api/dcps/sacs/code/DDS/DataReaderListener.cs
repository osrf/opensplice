﻿/*
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
 */

using System;
using System.Collections.Generic;
using DDS.OpenSplice.CustomMarshalers;

namespace DDS
{
    public abstract class DataReaderListener : IDataReaderListener
    {
        public virtual void OnRequestedDeadlineMissed(IDataReader entityInterface, RequestedDeadlineMissedStatus status)
        {
        }
        public virtual void OnRequestedIncompatibleQos(IDataReader entityInterface, RequestedIncompatibleQosStatus status)
        {
        }
        public virtual void OnSampleRejected(IDataReader entityInterface, SampleRejectedStatus status)
        {
        }
        public virtual void OnLivelinessChanged(IDataReader entityInterface, LivelinessChangedStatus status)
        {
        }
        public virtual void OnDataAvailable(IDataReader entityInterface)
        {
        }
        public virtual void OnSubscriptionMatched(IDataReader entityInterface, SubscriptionMatchedStatus status)
        {
        }
        public virtual void OnSampleLost(IDataReader entityInterface, SampleLostStatus status)
        {
        }
    }
}
