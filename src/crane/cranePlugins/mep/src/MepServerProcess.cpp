/*
 *    Copyright 2020 Huawei Technologies Co., Ltd.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "MepServerProcess.h"

namespace NS_CRANE {

#if 0
{
    "notificationType": "SerAvailabilityNotification",
    "serviceReferences": {
        "serName": "serviceA",
        "serInstanceId": "xxxxx",
        "state": "ACTIVE",
        "changeType": "ADDED"
    },
    "links": "/mep/service_mgmt/v1/subscribe"
} 
#endif

    MepClazzType MepServerProcess::ClazzType() {
        return MepClazzType::ServerProcess;
    }

    void MepServerProcess::doInteraction(MepProduceType type) {
        unused(type);
    }

    void MepServerProcess::Init() {
        auto opts = Http::Endpoint::options().threads(1);
        _httpEndpoint->init(opts);
        LOG_DEBUG("Subecriber Notify listen on port[ %s ]", _httpEndpoint->getPort().toString().c_str());
    }

    void MepServerProcess::Start() {
        _httpEndpoint->setHandler(_router.handler());
        _httpEndpoint->serve();
    }
}
