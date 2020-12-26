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

#ifndef  __CRANE_MEPSERVICESUBSCRIBER_H__
#define  __CRANE_MEPSERVICESUBSCRIBER_H__

#include <vector>
#include <map>

#include <pistache/router.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Util.h"
#include "AbsMepService.h"

using namespace std;
using namespace Pistache;
using namespace rapidjson;

namespace NS_CRANE {
/*
    class ServiceSubscriberInfo {
        public:
            ServiceSubscriberInfo(const string& sn, const string& ref, const ServiceStatus st)
                :serviceName(sn), callbackRef(ref), status(st) {}

            string              serviceName;
            string              callbackRef;
            ServiceStatus       status;
    };

    using ServicesSubscriberInfo = map<string, shared_ptr<ServiceSubscriberInfo>>;
*/
    class MepServiceSubscriber : public AbstractMepService, public enable_shared_from_this<MepServiceSubscriber> {
        public:
            static const std::string clazzType;

            MepServiceSubscriber(shared_ptr<AbstractMediator> mediator, std::string appInstanceId, const vector<string>& services) : AbstractMepService(mediator, appInstanceId) {
                _services = vector<string>(services);                
            }

            MepClazzType ClazzType() override;

            //void doInteraction() override;
            void doInteraction(MepProduceType type) override;

            ServicesSubscriberInfo& getServicesInfo() {return _servicesInfo;}

            void DoServiceNotify(const Rest::Request& request, Http::ResponseWriter response);

            void rspProcess(const web::http::status_code statusCode, web::http::http_headers& headers, const string& rspBody);

            void abnormity();

            const string getCallbackRef() {
                return "http://" + hostUrl_ + "/" + _CALLBACK_REF;
            }

            const string getCallbackRefResource() {
                return "/" + _CALLBACK_REF;
            }

        private:
           static const string          _SERVICE_PATH;
           static const string          _SERVICE_RESOURCE;
           static const string          _SERVICE_SUBSCRIBE_BODY;
           static const string          _CALLBACK_REF;
           
           vector<string>               _services;      //services provided by mep, which will be subscribe his status.
           ServicesSubscriberInfo       _servicesInfo;  //services status info through subscriber notify.
           unsigned                     _commpleted = 0;
    };
    
}
#endif