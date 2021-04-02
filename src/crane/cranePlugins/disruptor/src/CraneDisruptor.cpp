/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-04-01 09:12:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-04-02 10:24:49
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
 * @Date: 2021-04-01 09:12:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-04-01 09:40:43
 */

#include "CraneDisruptor.h"

namespace NS_CRANE {
unsigned int CraneDisruptor::init() {
    return CRANE_SUCC;
}

unsigned CraneDisruptor::start() {
    return CRANE_SUCC;
}
unsigned CraneDisruptor::stop() noexcept {
    return CRANE_SUCC;
}

unsigned CraneDisruptor::swap_up(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) {
    return CRANE_SUCC;
}

unsigned CraneDisruptor::swap_down(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) {
    return CRANE_SUCC;
}    
    
void CraneDisruptor::initialize(const Options& ops) {
    _threadNum = ops.threadNum();
    _eventFactory = [](){ return DisruptorEvent(); };

    switch(ops.taskSchedulerPolicy()) {
        case TaskSchedulerPolicy::ThreadPerTaskScheduler:
            this->_taskScheduler = make_shared<Disruptor::ThreadPerTaskScheduler>();
            break;
        case TaskSchedulerPolicy::RoundRobinThreadAffinedTaskScheduler:
            _taskScheduler = make_shared<Disruptor::RoundRobinThreadAffinedTaskScheduler>();
            break;
        default:
            _taskScheduler = make_shared<Disruptor::ThreadPerTaskScheduler>();
    }
    
    switch(ops.waitStrategy()) {
        case WaitStrategy::BlockingWaitStrategy:
            _waitStrategy = make_shared<Disruptor::BlockingWaitStrategy>();
            break;
        case WaitStrategy::TimeoutBlockingWaitStrategy:
            _waitStrategy = make_shared<Disruptor::TimeoutBlockingWaitStrategy>(
                ops.time4TimeoutBlockingWait());
            break;
        case WaitStrategy::SleepWaitStrategy:
            _waitStrategy = make_shared<Disruptor::SleepingWaitStrategy>(
                ops.retries4SleepWait());
            break;
        case WaitStrategy::YieldWaitStrategy:
            _waitStrategy = make_shared<Disruptor::YieldingWaitStrategy>();
            break;
        case WaitStrategy::SpinWaitWaitStrategy:
            _waitStrategy = make_shared<Disruptor::SpinWaitWaitStrategy>();
            break;
        default:
            _waitStrategy = make_shared<Disruptor::BlockingWaitStrategy>();
    }

    _disruptor = make_shared<Disruptor::disruptor<DisruptorEvent>>(
        _eventFactory,
        ops.ringBufferLen(), 
        _taskScheduler);
}

void CraneDisruptor::handler(EventHandlers& ds) {
    _disruptor->handleEventsWith(ds);
}

void CraneDisruptor::startUp() {
    _taskScheduler->start(_threadNum);
    _disruptor->start();
}

void CraneDisruptor::closeUp() {
    _disruptor->shutdown();
    _taskScheduler->stop();
}

void CraneDisruptor::publish(DisruptorEvent e) {
    auto ringBuffer = _disruptor->ringBuffer();
    auto nextSequence = ringBuffer->next();
    (*ringBuffer)[nextSequence].name = e.name;
    (*ringBuffer)[nextSequence].age = e.age;
    ringBuffer->publish(nextSequence);
}

CRANE_PLUGIN_DEFINE("1.0.0",    //插件接口版本
                    Itf_CraneDisruptor,
                    "1.0.0",    //要求的插件框架版本   
                    CraneDisruptor,
                    "crane system plugin, which is a high perfermence message bus.",
                    "dongyin@huawei.com",
                    "LGPL")  
}