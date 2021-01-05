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

#ifndef __CRANE_MEPMEDIATOR_H__
#define __CRANE_MEPMEDIATOR_H__

#include <iostream>
#include <memory>
#include <atomic>

#include <pistache/client.h>
#include <pistache/http.h>
#include <pistache/net.h>

#include "craneMacro.h"
#include "AbsMediator.h"
#include "AbsMepService.h"
#include "MepServiceReady.h"
#include "MepServiceDiscover.h"
#include "MepServiceRegister.h"
#include "MepServiceRoute.h"
#include "MepServiceSubscriber.h"
#include "MepServerProcess.h"

using namespace std;

namespace NS_CRANE {

    class MepMediator : public AbstractMediator , public enable_shared_from_this<MepMediator> {
        public:
            MepMediator();

            ~MepMediator() override;

            MepMediator& stuffRegService(vector<string>& regServices) {
                _regServices = regServices;
                for (auto s : _regServices) {
                    LOG_DEBUG("s: [ %s ]", s.c_str());
                }
                return *shared_from_this();
            }

            MepMediator& stuffSubServices(const vector<string>& subServices) {
                _subServices = subServices;
                for (auto s : _subServices) {
                    LOG_DEBUG("s: [ %s ]", s.c_str());
                }
                return *shared_from_this();
            }

            MepMediator& stuffDsvServices(const vector<string>& dsvServices) {
                _dsvServices = dsvServices;
                for (auto s : _dsvServices) {
                    LOG_DEBUG("s: [ %s ]", s.c_str());
                }
                return *shared_from_this();
            }

            /**
             * @Descripttion: MEP主控者override该方法，根据入参的class类型获知具体的MEP交互过程。
             *                调用该class对象的接口方法获取相关信息，然后控制后续行为。
             * @Param: 
             * @Return: 
             */            
            void MepServiceResult(shared_ptr<AbstractMepService>) override;

            void doAction() override;

            void doDiscover() override;
        
            /**
             * @Descripttion: 创建和MEP交互所需要的过程对象实例
             * @Param: 
             * @Return: 
             */            
            void CreateMepServices() override;

            const DiscoverServicesResult& discoverServicesResult() override {
                return _discoverServicesResult;
            }

            const ServicesSubscriberInfo& servicesSubscriberInfo() override {
                return _servicesSubscriberInfo;
            }

            void feedServicesSubscriberInfo(void* servicesInfo) override;

            
        private:
            string                              _appInstanceId;

            shared_ptr<MepServiceReady>         _srvRdy;
            shared_ptr<MepServiceDiscover>      _srvDsv;
            shared_ptr<MepServiceRegister>      _srvReg;
            shared_ptr<MepServiceRoute>         _srvRte;
            shared_ptr<MepServiceSubscriber>    _srvSub;
            shared_ptr<MepServerProcess>        _svrPrc;    //和API-GW交互的服务端

            vector<string>                      _dsvServices;   //需要发现的服务名称；
            vector<string>                      _regServices;   //需要注册的服务名称;
            vector<string>                      _subServices;   //需要订阅的服务名称;

            DiscoverServicesResult              _discoverServicesResult;    //存储服务发现的结果    
            RegisterServicesResult              _registerServicesResult;    //存储服务注册的结果
            ServicesSubscriberInfo              _servicesSubscriberInfo;    //存储服务订阅返回的信息

    };

}

#endif
