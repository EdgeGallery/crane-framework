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

#ifndef __PLUGIN_CRANEOBSERVER_H__
#define __PLUGIN_CRANEOBSERVER_H__

#include <string>
#include <vector>
#include <thread>
#include "craneMacro.h"
#include "Util.h"

#include "CraneTopic.h"
#include "CraneMsg.h"

using namespace std;
namespace NS_CRANE {

class CraneObserver {
    public:
        CraneObserver() {
            LOG_ENTER();
        }

        virtual ~CraneObserver() {}

        /**
         * @Descripttion:   subscribe the special topic.
         * @Param:          topic: Topic would be observe.
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        virtual unsigned subscribe(const CraneTopic& topic) = 0;

        /**
         * @Descripttion:   unsubscribe the special topic.
         * @Param:          topic: Topic of the data. 
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        virtual unsigned unsubscribe(const CraneTopic& topic) = 0;

        void Bind(void );

        //const CraneTopic& topic() const { return _topic; }

        /**
         * @Descripttion:   异步消息通知，将主题信息和数据的地址返回给观察者.
         * @Param:          topic: Topic of the data. 
         * @Param:          data: . 
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */       
        unsigned notify(const CraneTopic& topic, shared_ptr<CraneMsg> data) {
            doNotify(topic, data);
            return CRANE_SUCC;
        }

        /**
         * @Descripttion:   a special Oberver subClass should implement this method, and  
         *                  use dynamic_pointer_cast<D>(data) to obtain the special class type of data.
         * @Param:          topic: Topic of the data. 
         * @Param:          data: const share_ptr which is swapped const CraneMsg. 
         *                  Subclass should be dynamic_pointer_cast to special data type in override function.
         *                  Subclass could read this data at the same time, or save the share_ptr and read it 
         *                  when Oberver subclass need it. 
         *                  The data will stay in the queue for some time.
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */  
        //virtual unsigned doNotify(const CraneTopic& topic, const shared_ptr<const CraneMsg> data) {
        virtual unsigned doNotify(const CraneTopic& topic, shared_ptr<const CraneMsg> data) {
            unused(topic); unused(data);
            return CRANE_SUCC;
        }

        //用户同步消息更新；notify中会调用子类的doNotify方法，子类具体的更像动作;
        unsigned update(const CraneTopic& topic, shared_ptr<CraneMsg> data) {
            doUpdate(topic, data);
            return CRANE_SUCC;
        }

        /**
         * @Descripttion:   a special Oberver subClass should implement this method, and  
         *                  use dynamic_pointer_cast<D>(data) to obtain the special class type of data.
         * @Param:          topic: Topic of the data. 
         * @Param:          data: const share_ptr which is swapped const CraneMsg. 
         *                  Subclass should be dynamic_pointer_cast to special data type in override function, and
         *                  process the data at the same time.
         *                  The data will not be keep in the CraneMsgCenter.
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */ 
        virtual unsigned doUpdate(const CraneTopic& topic, shared_ptr<const CraneMsg> data) {
            unused(topic); unused(data);
            return CRANE_SUCC;
        }

};

}
#endif
