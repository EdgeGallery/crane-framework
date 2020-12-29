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

#ifndef __CRANE_ABSMEDIATOR_H___
#define __CRANE_ABSMEDIATOR_H___

#include <iostream>
#include <memory>

#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/details/basic_types.h>

#include "craneMacro.h"
#include "Util.h"

#include "CraneMepInterface.h"


using namespace std;

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace NS_CRANE {
    
    //AbstractMediator just used the pointer of AbstractMepService and will not call the member function of it.
    //So I just forward declaration AbstractMepService class and do not include h file.
    class AbstractMepService;

    class AbstractMediator {
        public:
            virtual ~AbstractMediator() {std::cout<<"~AbstractMediator()"<<std::endl;}

            /**
             * @Descripttion: MepService子类调用该方法，将和MEP交互的结果反馈给Mediator的实现类
             * @Param: AbstractMepService*
             * @Return: void
             */            
            virtual void MepServiceResult(shared_ptr<AbstractMepService>) = 0;

            /**
             * @Descripttion: 创建和MEP API-GW交互的过程对象实例
             * @Param: 
             * @Return: 
             */            
            virtual void CreateMepServices() = 0;

            /**
             * @Descripttion: 触发和MEP APIGW的交互过程，子类定义具体的实现细节
             * @Param: 
             * @Return: 
             */            
            virtual void doAction() = 0;

            virtual void doDiscover() = 0;

            /**
             * @Descripttion: Get mep services information which have been discovered. 
             * @Param: null
             * @Return: DiscoverServicesResult&
             */            
            virtual const DiscoverServicesResult& discoverServicesResult() = 0;

            virtual const ServicesSubscriberInfo& servicesSubscriberInfo() = 0;

            virtual void feedServicesSubscriberInfo(void* servicesInfo) {
                unused(servicesInfo);
            }

            //virtual const Pistache::Http::Client& getClient() = 0;

            //Pistache::Http::Client client;
            //shared_ptr<http_client> client;

    };

}


#endif
