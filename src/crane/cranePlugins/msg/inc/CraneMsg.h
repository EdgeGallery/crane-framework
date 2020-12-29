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

#ifndef __PLUGIN_CRANEMSG_H__
#define __PLUGIN_CRANEMSG_H__

#include <string>
#include <time.h>
#include <chrono>

#include "craneMacro.h"
#include "CraneTopic.h"

using namespace std;

namespace NS_CRANE {


class CraneMsg {
    public:         
        using Priority = unsigned char;
        using Key = size_t;      
        static const Priority DEFAULT_PRIORITY;
        static const time_t DEFAULT_AGING_TIME;

        CraneMsg(CraneTopic topic, Priority pri = DEFAULT_PRIORITY, time_t ageingTime = DEFAULT_AGING_TIME) : _topic(topic), _priority(pri), _ageingTime(ageingTime) {
            LOG_ENTER();
            chrono::time_point<chrono::system_clock, chrono::seconds> tp 
                = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());
            _timestamp = tp.time_since_epoch().count(); 
        }

        const CraneTopic& topic() const { return _topic; }

        const Priority& priority() const { return _priority; }

        const time_t& timestamp() const { return _timestamp; }

        const Key& key() const { return _key; }

        void setKey(Key key) { _key = key; }

        bool isStale(time_t now) {
            return ((now - _timestamp) > _ageingTime);
        }

        /**
         * @Descripttion:   调用Msg center方法将数据放置到主题对应的队列中.
         * @Param:          topic: Topic of the data. 
         * @Param:          oberver: a oberver which desire to get the data of the specified topic. 
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */    
        virtual unsigned publish() = 0;

        virtual ~CraneMsg() {}
    private:
        CraneTopic              _topic;
        const Priority          _priority; //0 is the highest priority and 255 is the lowest priority.
        time_t                  _timestamp;
        time_t                  _ageingTime; //seconds
        Key                     _key;   //key of the DatasPool.
};



class PriorityCompare {
    public:
        bool operator() (shared_ptr<CraneMsg> a, shared_ptr<CraneMsg> b) {
            //The expression comp(a,b), where comp is an object of this type and a and b are elements in the container, 
            //shall return true if a is considered to go before b in the strict weak ordering the function defines.
            return (a->priority() < b->priority());
        }
};

class AgeCompare {
    public:
        bool operator() (shared_ptr<CraneMsg> a, shared_ptr<CraneMsg> b) {
            //The expression comp(a,b), where comp is an object of this type and a and b are elements in the container, 
            //shall return true if a is considered to go before b in the strict weak ordering the function defines.
            return (a->timestamp() < b->timestamp());
        }
};

}

#endif
