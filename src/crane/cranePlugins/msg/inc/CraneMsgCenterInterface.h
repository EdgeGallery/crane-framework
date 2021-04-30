/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-27 08:56:17
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

#ifndef __PLUGIN_CRANEMSGCENTERINTERFACE_H__
#define __PLUGIN_CRANEMSGCENTERINTERFACE_H__

#include <memory>

#include "PluginBaseInterface.h"

#include "CraneTopic.h"
#include "CraneMsg.h"
#include "CraneObserver.h"

using namespace std;
namespace NS_CRANE {

class Itf_CraneMsgCenter : public PluginBase {
    public:
        /**
         * @Descripttion:   Subscribe a special topic with Observer implement class instance.
         * @Param:          topic: Topic of the data. 
         * @Param:          oberver: a oberver which desire to get the data of the specified topic. 
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        virtual unsigned subscriber(const CraneTopic& topic, shared_ptr<CraneObserver> observer) = 0;

        /**
         * @Descripttion:   Unsubscribe the topic with Observer implement class instance.
         * @Param:          topic: Topic of the data. 
         * @Param:          oberver: A oberver which has subscriber the data of the specified topic. 
         * @Param[out]:     null
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        virtual unsigned unSubscriber(const CraneTopic& topic, shared_ptr<CraneObserver> observer) = 0;

        /**
         * @Descripttion:   Publish the data to the special topic.
         * @Param:          topic: Topic of the data. 
         * @Param:          data: 
         * @Param[out]:     null
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        virtual unsigned publish(const CraneTopic& topic, shared_ptr<CraneMsg> data) = 0;

        /**
         * @Descripttion:   Pull the data back with key.
         * @Param:          key: key of datas poll.
         * @Param[out]:     data: reference of data.
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        //virtual unsigned pull(const CraneMsg::Key key, shared_ptr<CraneMsg>& data) = 0;
        virtual const shared_ptr<CraneMsg>& pull(const CraneMsg::Key key) = 0; 
};

}

#endif
