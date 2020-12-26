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

#ifndef __CRANE_CONCURRENTQUEUE_H__
#define __CRANE_CONCURRENTQUEUE_H__

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "craneMacro.h"

using namespace std;

namespace NS_CRANE {
    template<typename T>
    class ConcurrentQueue {
        private:
            queue<T> data;
            mutable mutex mtx;
            condition_variable cv;

        public:
            ConcurrentQueue() {}

            ConcurrentQueue(const ConcurrentQueue& o) {
                lock_guard<mutex> lock(o.mtx);
                data = o.data;
            }

            ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;

            /**
             * @Descripttion:   Push ref of data into the queue.
             * @Param:          val: reference of data.
             * @Param[out]:     null 
             * @Return:         void
             */    
            void push(const T& val) {
                lock_guard<mutex> lock(mtx);
                data.push(val);
                cv.notify_one();
            }

            /**
             * @Descripttion:   Push right ref of data into the queue.
             * @Param:          val: right reference of data.
             * @Param[out]:     null 
             * @Return:         void
             */    
            void push(const T&& val) {
                lock_guard<mutex> lock(mtx);
                data.push(val);
                cv.notify_one();
            }

            /**
             * @Descripttion:   Get the data into the front of queue.
             * @Param:          void.
             * @Param[out]:     null. 
             * @Return:         reference of the data.  
             */    
            const T& front() {
                lock_guard<mutex> lock(mtx);
                return data.front();
            }
            
            /*
            T& front() {
                lock_guard<mutex> lock(mtx);
                return data.front();
            }
            */

            void wait_pop(T& val) {
                unique_lock<std::mutex> ulock(mtx);
                cv.wait( ulock,  [this](){ return !data.empty(); } );
                val = data.front();
                data.pop();
            }

            shared_ptr<T> wait_pop() {
                unique_lock<std::mutex> ulock(mtx);
                cv.wait( ulock,  [this](){ return !data.empty(); } );
                shared_ptr<T> val(make_shared<T>(data.front()));
                data.pop();
                return val;
            }

            bool try_pop(T& val) {
                lock_guard<mutex> lock(mtx);
                if (data.empty()) { return false; } 
                val = data.front();
                data.pop();
                return true;
            }

            shared_ptr<T> try_pop() {
                lock_guard<mutex> lock(mtx);
                if (data.empty) { return shared_ptr<T>(); } 
                shared_ptr<T> val(make_shared<T>(data.front()));
                data.pop();
                return val;
            }

            bool empty() const {
                lock_guard<mutex> lock(mtx);
                return data.empty();
            }

            typename queue<T>::size_type size() const {
                lock_guard<mutex> lock(mtx);
                return data.size();
            }
    };
}

#endif