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

#include "CraneMsgCenter.h"

namespace NS_CRANE {
    unsigned CraneMsgCenter::init() {
        //fork a thread to seek the _topicMsgQueue, notify the CraneObserver which is find in _topicObervers by topic with
        //type of ASYNC.
        thread scanThread(&CraneMsgCenter::scanning, this);
        scanThread.detach();

        thread agingThread(&CraneMsgCenter::aging, this);
        agingThread.detach();

        return CRANE_SUCC;
    }

    void CraneMsgCenter::scanning() {
        hash<shared_ptr<CraneMsg>> hashFunc;
        while(true) {            
            {
                lock_guard<mutex> lock(_mtx);
                for (auto mItr = _topicMsgQueues.begin(); mItr != _topicMsgQueues.end(); ++mItr) {
                    //遍历各个topic下的消息队列
                    if (mItr->second.empty()) { 
                        this_thread::yield();
                        continue; 
                    }

                    CraneTopic topic = mItr->first;
                    //阻塞获取队列头部的消息
                    shared_ptr<CraneMsg> msg;
                    if (mItr->second.try_pop(msg) == true) {

                        //遍历该订阅该topic的观察者
                        pair<TopicObservers::iterator, TopicObservers::iterator> range(_topicObservers.equal_range(mItr->first));
                        for (TopicObservers::iterator itr = range.first; itr != range.second; ++itr) {
                            if (itr->second != shared_ptr<CraneObserver>()) {
                                //调用观察者的notify方法，将消息的shared_ptr<CraneMsg>对象
                                size_t key = hashFunc(msg);
                                msg->setKey(key);
                                _msgAgingQueue.push(msg);
                                _datasPool.put(key, msg);
                                
                                itr->second->notify(topic, msg);
                                LOG_DEBUG("Notify msg of topic { %s } to observer { %p }", topic.name().c_str(), itr->second.get());
                            }
                        } 
                    } else {
                        continue;
                    }           
                }
            }
            //this_thread::sleep_for(chrono::milliseconds(20));
            this_thread::yield();
        }
    }

    void CraneMsgCenter::aging() {
        shared_ptr<CraneMsg> msg;
        chrono::time_point<chrono::system_clock, chrono::seconds> tp;
        time_t timeNow;

        size_t count = 0;
        size_t workLoad = 0;

        while(true) {
            if (_msgAgingQueue.empty()) {
                this_thread::yield();
                continue;
            }

            tp = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());
            timeNow = tp.time_since_epoch().count();

            count = 0;
            workLoad = (_msgAgingQueue.size()>>2 < 128)? _msgAgingQueue.size()>>2 : 128;
            workLoad = (workLoad == 0)? 128 : workLoad;

            while (true) {
                msg = _msgAgingQueue.top();
                if (msg->isStale(timeNow)) {
                    _datasPool.del(msg->key());
                    _msgAgingQueue.wait_pop();
                    if (++count > workLoad) {
                        break;
                    }
                } else {
                    break;
                }
            }
            this_thread::yield();
        }
    }

    unsigned CraneMsgCenter::subscriber(const CraneTopic& topic, shared_ptr<CraneObserver> observer) {
        LOG_ENTER();

        if (topic.type() == CraneTopic::Type::ASYNC) {
            LOG_DEBUG("Topic type is ASYNC");

            ////////--------------LOCKED--------------////////
            lock_guard<mutex> lock(_mtx);

            TopicMsgQueues::iterator itr = _topicMsgQueues.find(topic);
            if (itr == _topicMsgQueues.end()) {
                //当前没有任会对该topic的订阅，则添加topic和对应的queue；
                DataQueue queue;
                _topicMsgQueues.insert(make_pair(topic, queue));
                LOG_DEBUG("Insert the msgQueue of the topic { %s } into the _topicMsgQueues.", topic.name().c_str());
            }

            //使用该topic message queue的观察者数量加一, 当引用计数为0时，从_topicMsgQueues中删除topic和queue；
            _topicMsgQueuesRef.find(topic)->second++;
            ////////--------------UNLOCKED--------------////////
        }

        bool isExist = false;
        {
            ////////--------------LOCKED--------------////////
            lock_guard<mutex> lock(_mtx);

            pair<TopicObservers::iterator, TopicObservers::iterator> range(_topicObservers.equal_range(topic));
            for (TopicObservers::iterator itr = range.first; itr != range.second; ++itr) {
                if (itr->second == observer) {
                    //已经该观察者已经订阅了该topic；
                    LOG_DEBUG("Observer { %p } has subscribed the topic { %s }", &observer, topic.name().c_str());
                    isExist = true;
                    break; 
                }
            }
            ////////--------------UNLOCKED--------------////////
        }

        //如果不存在，则将topic和observer添加到multimap中
        if (isExist == false) {
            ////////--------------LOCKED--------------////////
            lock_guard<mutex> lock(_mtx);

            _topicObservers.insert(make_pair(topic, observer));
            LOG_DEBUG("Insert observer { %p } into _topicObservers with the topic { %s }", &observer, topic.name().c_str());
            ////////--------------UNLOCKED--------------////////
        }

        return CRANE_SUCC;
    }

    unsigned  CraneMsgCenter::unSubscriber(const CraneTopic& topic, shared_ptr<CraneObserver> observer) {
        {
            //从_topicObservers的multimap中删除观察者
            ////////--------------LOCKED--------------////////
            lock_guard<mutex> lock(_mtx);

            pair<TopicObservers::iterator, TopicObservers::iterator> range(_topicObservers.equal_range(topic));
            for (TopicObservers::iterator itr = range.first; itr != range.second; ++itr) {
                if (itr->second == observer) {
                    //已经该观察者已经订阅了该topic；
                    _topicObservers.erase(itr);
                    break; 
                }
            }
            ////////--------------UNLOCKED--------------////////
        }

        if (topic.type() == CraneTopic::Type::ASYNC) {
            LOG_DEBUG("This is ASYNC topic, try to clean the _topicMsgQueueRef and _topicMsgQueues.");
            //如果是异步的topic，则将引用计数减一，如果最后结果为0，则从_topicMsgQueues中删除topic和queue；
            ////////--------------LOCKED--------------////////
            lock_guard<mutex> lock(_mtx);

            TopicMsgQueuesRef::iterator itr = _topicMsgQueuesRef.find(topic);
            if (itr != _topicMsgQueuesRef.end()) {
                if (itr->second > 0) { itr->second--; }
                if (itr->second == 0) {
                    //没有观察者使用该消息队列了，则删除这个map pair；
                    LOG_DEBUG("Erase the topic { %s } from _topicMsgQueuesRef.", topic.name().c_str());
                    _topicMsgQueuesRef.erase(itr);
                    TopicMsgQueues::iterator i = _topicMsgQueues.find(topic);
                    if (i != _topicMsgQueues.end()) {
                    LOG_DEBUG("Erase the topic { %s } and queue from _topicMsgQueues.", topic.name().c_str());
                        _topicMsgQueues.erase(i);
                    }
                }
            }
            ////////--------------UNLOCKED--------------////////
        }
        return CRANE_SUCC;
    }

    unsigned CraneMsgCenter::publish(const CraneTopic& topic, shared_ptr<CraneMsg> data) {
        if (topic.type() == CraneTopic::Type::ASYNC) {
            TopicMsgQueues::iterator itr;
            {
                //TODO: !!!需要将这里的锁去掉，使用_topicMsgQueues的成员函数中的锁，否则会存在加两次锁的问题。
                ////////--------------LOCKED--------------////////
                lock_guard<mutex> lock(_mtx);
                //_mtx.lock();

                itr = _topicMsgQueues.find(topic);
                if (itr == _topicMsgQueues.end()) {
                    LOG_DEBUG("There is not topic { %s } in the _topicMsgQueues, so insert pair of topic and queue.", topic.name().c_str());
                    DataQueue queue;
                    _topicMsgQueues.insert(make_pair(topic, queue));
                }

                itr = _topicMsgQueues.find(topic);
                if (itr != _topicMsgQueues.end()) {
                    itr->second.push(move(data));
                    return CRANE_SUCC;
                } else {
                    LOG_ERROR("Publish data failed!");
                    return CRANE_FAIL;
                }
                ////////--------------UNLOCKED--------------////////
            }
        } else {
            //TODO: !!!需要将这里的锁去掉，使用_topicMsgQueues的成员函数中的锁，否则会存在加两次锁的问题。
            ////////--------------LOCKED--------------////////
            lock_guard<mutex> lock(_mtx);

            pair<TopicObservers::iterator, TopicObservers::iterator> range(_topicObservers.equal_range(topic));
            for (TopicObservers::iterator itr = range.first; itr != range.second; ++itr) {
                if (itr->first == topic) {
                    //Observer will process the data in the update method. 
                    itr->second->update(topic, data);
                    LOG_DEBUG("Update topic { %s }'s data to observer.", topic.name().c_str());
                }
            }
            ////////--------------UNLOCKED--------------////////
        } 

        return CRANE_SUCC;
    }

    const shared_ptr<CraneMsg>& CraneMsgCenter::pull(const CraneMsg::Key key) {
        return _datasPool.get(key);
    }

    CRANE_PLUGIN_DEFINE_SINGLETON("1.0.0",    //插件接口版本
                    Itf_CraneMsgCenter,
                    "1.0.0",    //要求的插件框架版本   
                    CraneMsgCenter,
                    "crane system plugin, which is a message center.",
                    "dongyin@huawei.com",
                    "LGPL")
}
