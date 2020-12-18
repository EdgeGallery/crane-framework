/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-13 15:01:01
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 18:25:08
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