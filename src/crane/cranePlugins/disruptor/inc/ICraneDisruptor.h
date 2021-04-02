/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-03-31 19:38:45
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-04-02 17:04:12
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

/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-03-31 19:38:45
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-31 19:39:19
 */

#ifndef __PLUGIN_ICRANE_DISRUPTOR_H__
#define __PLUGIN_ICRANE_DISRUPTOR_H__

#include "cranePlugin.h"

#include "Disruptor/Disruptor.h"
#include "Disruptor/IEventHandler.h"
#include "Disruptor/ThreadPerTaskScheduler.h"
#include "Disruptor/RoundRobinThreadAffinedTaskScheduler.h"
#include "Disruptor/TimeoutBlockingWaitStrategy.h"
#include "Disruptor/SleepingWaitStrategy.h"
#include "Disruptor/YieldingWaitStrategy.h"
#include "Disruptor/SpinWaitWaitStrategy.h"


using namespace std;

namespace NS_CRANE {

constexpr size_t DISRUPTOR_RING_BUFFER_LEN = 1024 * 1024; 
constexpr size_t TIME_4_TIMEOUT_BLOCKING_WAIT_STRATEGY = 500;
constexpr int32_t RETRIES_4_SLEEP_WAIT_STRATEGY = 200;

    
template < typename E >
class Itf_CraneDisruptor : public PluginBase, public PluginSwappable {
public:
    enum class TaskSchedulerPolicy {
        ThreadPerTaskScheduler = 0,
        RoundRobinThreadAffinedTaskScheduler,
    };
        
    enum class WaitStrategy {
        BlockingWaitStrategy = 0,
        TimeoutBlockingWaitStrategy,
        SleepWaitStrategy,
        YieldWaitStrategy,
        SpinWaitWaitStrategy,
    };
    
    struct Options {
    public:
        Options() : 
            _ring_buffer_len(DISRUPTOR_RING_BUFFER_LEN),
            _taskSchedulerPolicy(TaskSchedulerPolicy::ThreadPerTaskScheduler), 
            _waitStrategyPolicy(WaitStrategy::BlockingWaitStrategy),
            _time4TimeoutBlockingWait(TIME_4_TIMEOUT_BLOCKING_WAIT_STRATEGY),
            _retries4SleepWait(RETRIES_4_SLEEP_WAIT_STRATEGY) {
                _threadNum = std::max(thread::hardware_concurrency()/2, 1u);
            }
    
        Options& ringBufferLen(size_t l) {
            _ring_buffer_len = l;
            return *this;
        }
    
        Options& taskSchedulerPolicy(TaskSchedulerPolicy p) {
            _taskSchedulerPolicy = p;
            return *this;
        }
    
        Options& waitStrategy(WaitStrategy s) {
            _waitStrategyPolicy = s;
            return *this;
        }
    
        Options& time4TimeoutBlockingWait(Disruptor::ClockConfig::Duration t) {
            _time4TimeoutBlockingWait = t;
            return *this;
        }
        Options& retries4SleepWait(int32_t c) {
            _retries4SleepWait = c;
            return *this;
        }
    
        Options& threadNum(uint32_t n) {
            _threadNum = n;
            return *this;
        }
    
        long ringBufferLen() const { 
            return _ring_buffer_len; 
        }
    
        TaskSchedulerPolicy taskSchedulerPolicy() const {
            return _taskSchedulerPolicy;
        }
    
        WaitStrategy waitStrategy() const {
            return _waitStrategyPolicy; 
        }
    
        Disruptor::ClockConfig::Duration time4TimeoutBlockingWait() const {
            return _time4TimeoutBlockingWait;
        }
    
        int32_t retries4SleepWait() const {
            return _retries4SleepWait;
        }
    
        uint32_t threadNum() const {
            return _threadNum;
        }
    
    private:
        size_t                  _ring_buffer_len;
        TaskSchedulerPolicy     _taskSchedulerPolicy;
        WaitStrategy            _waitStrategyPolicy;
        Disruptor::ClockConfig::Duration   _time4TimeoutBlockingWait;
        int32_t                 _retries4SleepWait;
        uint32_t                _threadNum;
    };

public:
    using EventHandlers = vector<shared_ptr<Disruptor::IEventHandler<E>>>;

    virtual void initialize(const Options&) = 0;
    virtual void startUp() = 0;
    virtual void closeUp() = 0;
    virtual void handler(vector<shared_ptr<Disruptor::IEventHandler<E>>>&) = 0;
    virtual void publish(E) = 0;
};

}

#endif