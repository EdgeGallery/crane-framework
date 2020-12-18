#ifndef __CRANE_CONCURRENTPRIORITYQUEUE_H__
#define __CRANE_CONCURRENTPRIORITYQUEUE_H__

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "craneMacro.h"

using namespace std;

namespace NS_CRANE {
    template<typename T, typename C>
    class ConcurrentPriorityQueue {
        private:
            priority_queue<T, std::vector<T>, C> _data;
            mutable mutex _mtx;
            condition_variable _cv;

        public:
            ConcurrentPriorityQueue() {}

            ConcurrentPriorityQueue(const ConcurrentPriorityQueue& o) {
                lock_guard<mutex> lock(o._mtx);
                _data = o._data;
            }

            ConcurrentPriorityQueue& operator=(const ConcurrentPriorityQueue&) = delete;

            /**
             * @Descripttion:   Push ref of data into the queue.
             * @Param:          val: reference of data.
             * @Param[out]:     null 
             * @Return:         void
             */    
            void push(const T& val) {
                lock_guard<mutex> lock(_mtx);
                _data.push(val);
                _cv.notify_one();
            }

            /**
             * @Descripttion:   Push right ref of data into the queue.
             * @Param:          val: right reference of data.
             * @Param[out]:     null 
             * @Return:         void
             */    
            void push(const T&& val) {
                lock_guard<mutex> lock(_mtx);
                _data.push(val);
                _cv.notify_one();
            }

            /**
             * @Descripttion:   Get the data into the front of queue.
             * @Param:          void.
             * @Param[out]:     null. 
             * @Return:         reference of the data.  
             */    
            const T& top() {
                lock_guard<mutex> lock(_mtx);
                return _data.top();
            }
            
            /*
            T& front() {
                lock_guard<mutex> lock(mtx);
                return data.front();
            }
            */

            void wait_pop(T& val) {
                unique_lock<std::mutex> ulock(_mtx);
                _cv.wait( ulock,  [this](){ return !_data.empty(); } );
                val = _data.top();
                _data.pop();
            }

            shared_ptr<T> wait_pop() {
                unique_lock<std::mutex> ulock(_mtx);
                _cv.wait( ulock,  [this](){ return !_data.empty(); } );
                shared_ptr<T> val(make_shared<T>(_data.top()));
                _data.pop();
                return val;
            }

            bool try_pop(T& val) {
                lock_guard<mutex> lock(_mtx);
                if (_data.empty()) { return false; } 
                val = _data.top();
                _data.pop();
                return true;
            }

            shared_ptr<T> try_pop() {
                lock_guard<mutex> lock(_mtx);
                if (_data.empty) { return shared_ptr<T>(); } 
                shared_ptr<T> val(make_shared<T>(_data.top()));
                _data.pop();
                return val;
            }

            bool empty() const {
                lock_guard<mutex> lock(_mtx);
                return _data.empty();
            }

            typename queue<T>::size_type size() const {
                lock_guard<mutex> lock(_mtx);
                return _data.size();
            }
    };
}

#endif