/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-27 08:57:29
 */
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

#ifndef __PLUGIN_CRANEMEPINTERFACE_H__
#define __PLUGIN_CRANEMEPINTERFACE_H__

#include <memory>

#include "PluginBaseInterface.h"
#include "Util.h"

using namespace std;
namespace NS_CRANE {
    class MepService {
        public:
            MepService(string id, string name) : srvId(id), srvName(name) {}
            string      srvId;
            string      srvName;
            string      version;
            string      state;
            using Endpoints = std::list<string>; //string is include ip and port such as 1.1.1.1:5060
            Endpoints    endpoints;
    };
    using DiscoverServicesResult = std::map<std::string, shared_ptr<MepService>>;

    enum class ServiceStatus {
        INVALID = 0,    //代码处理的异常返回状态
        ACTIVE,
        INACTIVE,
        UNKNOW,     //尚未发起订阅；
        PENDING,    //发起了订阅，但为收到订阅通知信息；
    };
    class ServiceSubscriberInfo {
        public:
            ServiceSubscriberInfo(const string& sn, const string& ref, const ServiceStatus st)
                :serviceName(sn), callbackRef(ref), status(st) {}

            string              serviceName;
            string              callbackRef;
            ServiceStatus       status;
    };
    using ServicesSubscriberInfo = map<string, shared_ptr<ServiceSubscriberInfo>>;    
    
class Itf_CraneMep : public PluginBase {
    public:
        /**
         * @Descripttion: 触发和MEP的交互动作，包括服务注册、发现、订阅等行为。该方法在进程加载的时候调用。
         * @Param: 
         * @Return: 
         */    
        virtual unsigned invoke() = 0;

        /**
         * @Descripttion: 手动触发服务发现过程，这里只是一个例子，该插件的继承类可以定义并实现其他的doXXXXXX()接口。
         * @Param: 
         * @Return: 
         */    
        virtual unsigned doDiscover() = 0;

        /**
         * @Descripttion: Fetch services information which have been discover.
         * @Param: 
         * @Return: 
         */    
        virtual const DiscoverServicesResult& fetchServices() = 0;

        /**
         * @Descripttion: Fetch services status notify information response from mep.
         * @Param: null 
         * @Return: ServicesSubscriberInfo
         */        
        virtual const ServicesSubscriberInfo& fetchSubNtfInfo() = 0;
};

}

#endif
