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

#ifndef __CRANE_MEPSERVICEREGISTER_H__
#define __CRANE_MEPSERVICEREGISTER_H__

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "AbsMepService.h"

using namespace std;
//using namespace Pistache;
using namespace rapidjson;

namespace NS_CRANE {

    class RegisterService {
        public:
            RegisterService(string id, string name) : srvId(id), srvName(name) {}
            string              srvId;
            string              srvName;
            string              location;
            ServiceStatus       status;
    };

    using RegisterServicesResult = map<string, shared_ptr<RegisterService>>;

    class MepServiceRegister : public AbstractMepService, public enable_shared_from_this<MepServiceRegister> {
        public:

            MepServiceRegister(shared_ptr<AbstractMediator> mediator, std::string appInstanceId, const vector<string>& services) : AbstractMepService(mediator, appInstanceId) {
                _services = services;    
            }

            MepClazzType ClazzType() override;

            //void doInteraction() override;
            void doInteraction(MepProduceType type) override;

            const RegisterServicesResult& getRegisterResult() {
                return _servicesInfo;
            }

            void rspProcess(const web::http::status_code statusCode, web::http::http_headers& headers, const string& rspBody);

            void abnormity();
            
        private:
           static const std::string     _SERVICE_REGISTER_BODY;
           static const string          _SERVICE_PATH;
           static const string          _SERVICE_RESOURCE;

           vector<string>               _services;    //需要注册的服务名称
           
           RegisterServicesResult       _servicesInfo;

           //Commpleted service discover process number.
           unsigned                     _commpleted = 0;
    };

}

#endif
