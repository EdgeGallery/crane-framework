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

#ifndef __CRANE_MEPSERVICEDISCOVER_H__
#define __CRANE_MEPSERVICEDISCOVER_H__

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Log.h"

#include "CraneMepInterface.h"
#include "AbsMepService.h"

using namespace std;
using namespace rapidjson;

namespace NS_CRANE {

    class MepServiceDiscover : public AbstractMepService, public enable_shared_from_this<MepServiceDiscover>{
        public:
    
            MepServiceDiscover(shared_ptr<AbstractMediator> mediator, 
                                std::string appInstanceId, 
                                const vector<string>& services) : AbstractMepService(mediator, appInstanceId) {
                _services = services;

                //Show the services name which is needed to discovered. it's just for debug.
                for (auto s : _services) {
                    
                    LOG_DEBUG("Need to dsicover the service: [ %s ]", s.c_str());
                } 
            }

            MepClazzType ClazzType() override;

            void doInteraction(MepProduceType type) override;

            const DiscoverServicesResult& getDiscoverResult() {
                return _servicesInfo;
            }

            void rspSingleServiceProcess(const string& rspBody, const web::http::status_code statusCode);

            void rspAllServiceProcess(const string& rspBody, const web::http::status_code statusCode);

            void abnormity();

        private:
            static const string     _SERVICE_PATH;
            static const string     _PARA_NAME; 

            vector<string>          _services;
            DiscoverServicesResult  _servicesInfo;//Save the information of services fatched from MEP API-GW.
            unsigned                _commpleted = 0;//Commpleted service discover process number.
    };
}

#endif
