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

#ifndef __PLUGIN_CRANEMEP_H__
#define __PLUGIN_CRANEMEP_H__

#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

#include "Log.h"
#include "Util.h"

#include "cranePlugin.h"
#include "CraneMepInterface.h"
#include "MepMediator.h"

using namespace std;

namespace NS_CRANE {
    class CraneMep : public Itf_CraneMep {
    public:
        static const string CONFIG_FILENAME;

        CraneMep() { }

        virtual ~CraneMep() { }
        
        unsigned init() override;

        /**
         * @Descripttion: Invoke produre of interaction with mep. This method should be invoke during system has initialized.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAIL
         */    
        unsigned invoke() override;

        /**
         * @Descripttion: 手动触发服务发现过程，这里只是一个例子，该插件的继承类可以定义并实现其他的doXXXXXX()接口。
         * @Param: 
         * @Return: 
         */    
        virtual unsigned doDiscover() override;

        /**
         * @Descripttion: Fetch services information which have been discovered.
         * @Param: null
         * @Return: DiscoverServicesResult&: services information. 
         */    
        virtual const DiscoverServicesResult& fetchServices() override;

        /**
         * @Descripttion: Fetch services status notify information response from mep.
         * @Param: null 
         * @Return: ServicesSubscriberInfo
         */        
        virtual const ServicesSubscriberInfo& fetchSubNtfInfo() override;

    private:
        unsigned _parseConfigFile();

    private:
        shared_ptr<AbstractMediator>        _mepMediator;
        vector<string>                      _regServices;
        vector<string>                      _dsvServices;
        vector<string>                      _subServices;        
    };

}

#endif
