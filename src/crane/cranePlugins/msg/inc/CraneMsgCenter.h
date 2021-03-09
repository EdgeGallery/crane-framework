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

#ifndef __PLUGIN_CRANEMSGCENTER_H__
#define __PLUGIN_CRANEMSGCENTER_H__

#include <memory>
#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include <time.h>
#include <chrono>
#include <functional>

#include "cranePlugin.h"
#include "ConcurrentPriorityQueue.h"
#include "ConcurrentHashMap.h"

#include "CraneMsgCenterInterface.h"

using namespace std;

namespace NS_CRANE {

class CraneMsgCenter : public Itf_CraneMsgCenter {
    public:
        using DataQueue = ConcurrentPriorityQueue<shared_ptr<CraneMsg>, PriorityCompare>;
        using TopicMsgQueues = map<CraneTopic, DataQueue>;
        using TopicMsgQueuesRef = map<CraneTopic, unsigned>;
        using TopicObservers = multimap<CraneTopic, shared_ptr<CraneObserver>>;
        using DatasAgingQueue = ConcurrentPriorityQueue<shared_ptr<CraneMsg>, AgeCompare>;
        using DatasHashMap = ConcurrentHashMap<size_t, shared_ptr<CraneMsg>>;

        /**
         * @Descripttion:   class instance.
         * @Param:          topic: Topic of the data. 
         * @Param:          oberver: a oberver which desire to get the data of the specified topic. 
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        unsigned init() override;

        unsigned start() override;

        unsigned stop() noexcept override;

        /**
         * @Descripttion:   Subscribe a special topic with Observer implement class instance.
         * @Param:          topic: Topic of the data. 
         * @Param:          oberver: a oberver which desire to get the data of the specified topic. 
         * @Param[out]:     null 
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        unsigned subscriber(const CraneTopic& topic, shared_ptr<CraneObserver> observer) override;

        /**
         * @Descripttion:   Unsubscribe the topic with Observer implement class instance.
         * @Param:          topic: Topic of the data. 
         * @Param:          oberver: A oberver which has subscriber the data of the specified topic. 
         * @Param[out]:     null
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        unsigned unSubscriber(const CraneTopic& topic, shared_ptr<CraneObserver> observer) override;

        /**
         * @Descripttion:   Publish the data to the special topic.
         * @Param:          topic: Topic of the data. 
         * @Param:          data: 
         * @Param[out]:     null
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        unsigned publish(const CraneTopic& topic, shared_ptr<CraneMsg> data) override;

        /**
         * @Descripttion:   Pull the data back with key, which is invoked by another thread of the observer.
         * @Param:          key: key of datas poll.
         * @Param[out]:     data: reference of data.
         * @Return:         CRANE_SUCC/CRANE_FAIL 
         */        
        //unsigned pull(const CraneMsg::Key key, shared_ptr<CraneMsg>& data) override;
        const shared_ptr<CraneMsg>& pull(const CraneMsg::Key key) override; 

    private:
        void scanning();

        void aging();

    private:
        TopicMsgQueues      _topicMsgQueues;        //存储topic--->priorityQueue<CraneMsg>的map
        TopicMsgQueuesRef   _topicMsgQueuesRef;
        TopicObservers      _topicObservers;        //存储topic--->observer的map
        DatasAgingQueue     _msgAgingQueue;         //数据老化队列，
        DatasHashMap        _datasPool;             //已经notify的异步消息存储在该数据结构中，数据老化后，则从中删除


        mutable mutex       _mtx;

};

}

#endif
