/*
 * @Descripttion: 定义和MEP交互的抽象行为
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-01 19:42:49
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 18:49:46
 */ 
#ifndef __CRANE_ABSMEDIATOR_H___
#define __CRANE_ABSMEDIATOR_H___

#include <iostream>
#include <memory>

//#include <pistache/client.h>
//#include <pistache/http.h>
//#include <pistache/net.h>

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
