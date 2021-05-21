/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: hwdy
 * @Date: 2021-03-31 19:38:15
 * @LastEditors: hwdy
 * @LastEditTime: 2021-04-08 10:07:01
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
 * @Author: hwdy
 * @Date: 2021-03-31 19:38:15
 * @LastEditors: hwdy
 * @LastEditTime: 2021-03-31 19:38:15
 */

#ifndef __PLUGIN_CRANE_DISRUPTOR_H__
#define __PLUGIN_CRANE_DISRUPTOR_H__

#include "ICraneDisruptor.h"

using namespace std;

namespace NS_CRANE {

template<typename E>
class CraneDisruptor : public Itf_CraneDisruptor<E> {
public:
    unsigned int init() override {
        return CRANE_SUCC;
    }
    unsigned start() override {
        return CRANE_SUCC;
    }
    unsigned stop() noexcept override {
        return CRANE_SUCC;
    }

    unsigned swap_up(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) override {
        unused(stale, fresh);
        return CRANE_SUCC;
    }
    unsigned swap_down(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) override {
        unused(stale, fresh);
        return CRANE_SUCC;
    }

    void initialize(const typename Itf_CraneDisruptor<E>::Options& ops) override {
        _threadNum = ops.threadNum();
        _producerType = ops.producerType();
        _eventFactory = [](){ return E(); };

        _taskScheduler = taskPolicys.at(ops.taskSchedulerPolicy()).taskScheduler();
        _waitStrategy = createWaitStrategys(ops).at(ops.waitStrategy()).waitStrategy();

        _disruptor = make_shared<Disruptor::disruptor<E>>(
            _eventFactory,
            ops.ringBufferLen(), 
            _taskScheduler,
            _producerType,
            _waitStrategy
        );
    }

    void handler(typename Itf_CraneDisruptor<E>::EventHandlers& ds) override {
        _disruptor->handleEventsWith(ds);        
    }
    
    void startUp() override {
        _taskScheduler->start(_threadNum);
        _disruptor->start();    
    }

    void closeUp() override {
        _disruptor->shutdown();
        _taskScheduler->stop();
    }

    void publish(E&& e) override {
        auto ringBuffer = _disruptor->ringBuffer();
        auto nextSequence = ringBuffer->next();
        (*ringBuffer)[nextSequence].name = e.name;
        (*ringBuffer)[nextSequence].age = e.age;
        ringBuffer->publish(nextSequence); 
    }

    virtual ~CraneDisruptor() { cout << "~CraneDisruptor()" << endl; }
    
    private:
        function<E()>                           _eventFactory;
        Disruptor::ProducerType                 _producerType;
        shared_ptr<Disruptor::ITaskScheduler>   _taskScheduler;
        shared_ptr<Disruptor::IWaitStrategy>    _waitStrategy;
        shared_ptr<Disruptor::disruptor<E>>     _disruptor;
        uint32_t                                _threadNum;

        using TaskSchedulerPolicy = typename Itf_CraneDisruptor<E>::TaskSchedulerPolicy;
        struct ITaskSchedulerPolicy {
            virtual shared_ptr<Disruptor::ITaskScheduler> taskScheduler() const {
                return make_shared<Disruptor::ThreadPerTaskScheduler>();
            }
        };
        struct ThreadPerTaskSchedulerPolicy : public ITaskSchedulerPolicy {
            shared_ptr<Disruptor::ITaskScheduler> taskScheduler() const override {
                    return make_shared<Disruptor::ThreadPerTaskScheduler>();
            }
        };
        struct RoundRobinThreadAffinedTaskSchedulerPolicy : public ITaskSchedulerPolicy {
            shared_ptr<Disruptor::ITaskScheduler> taskScheduler() const override {
                    return make_shared<Disruptor::RoundRobinThreadAffinedTaskScheduler>();
            }
        };
        const map<const TaskSchedulerPolicy, const ITaskSchedulerPolicy> taskPolicys = {
            {TaskSchedulerPolicy::ThreadPerTaskScheduler, ThreadPerTaskSchedulerPolicy()},
            {TaskSchedulerPolicy::RoundRobinThreadAffinedTaskScheduler, RoundRobinThreadAffinedTaskSchedulerPolicy()}
        };

        struct IWaitStrategy {
            virtual shared_ptr<Disruptor::IWaitStrategy> waitStrategy() const {
                return make_shared<Disruptor::BlockingWaitStrategy>();
            }
        };
        struct BlockingWaitStrategy : public IWaitStrategy {
            shared_ptr<Disruptor::IWaitStrategy> waitStrategy() const override {
                return make_shared<Disruptor::BlockingWaitStrategy>();
            }
        };
        struct TimeoutBlockingWaitStrategy : public IWaitStrategy {
            TimeoutBlockingWaitStrategy(Disruptor::ClockConfig::Duration d) : _d(d) {}
            shared_ptr<Disruptor::IWaitStrategy> waitStrategy() const override {
                return make_shared<Disruptor::TimeoutBlockingWaitStrategy>(_d);
            }
        private:
            Disruptor::ClockConfig::Duration _d;
        };
        struct SleepWaitStrategy : public IWaitStrategy {
            SleepWaitStrategy(uint32_t t) : _t(t) {}
            shared_ptr<Disruptor::IWaitStrategy> waitStrategy() const override {
                return make_shared<Disruptor::SleepingWaitStrategy>(_t);
            }
        private:
            uint32_t    _t;
        };
        struct YieldWaitStrategy : public IWaitStrategy {
            shared_ptr<Disruptor::IWaitStrategy> waitStrategy() const override {
                return make_shared<Disruptor::YieldingWaitStrategy>();
            }
        };
        struct SpinWaitWaitStrategy : public IWaitStrategy {
            shared_ptr<Disruptor::IWaitStrategy> waitStrategy() const override {
                return make_shared<Disruptor::SpinWaitWaitStrategy>();
            }
        };
        using WaitStrategy = typename Itf_CraneDisruptor<E>::WaitStrategy;
        using Options = typename Itf_CraneDisruptor<E>::Options;
        const map<const WaitStrategy, const IWaitStrategy>
        createWaitStrategys(const Options& ops) {
            return map<const WaitStrategy, const IWaitStrategy> {
                {WaitStrategy::BlockingWaitStrategy, BlockingWaitStrategy()},
                {WaitStrategy::TimeoutBlockingWaitStrategy, TimeoutBlockingWaitStrategy(ops.time4TimeoutBlockingWait())},
                {WaitStrategy::SleepWaitStrategy, SleepWaitStrategy(ops.retries4SleepWait())},
                {WaitStrategy::YieldWaitStrategy, YieldWaitStrategy()},
                {WaitStrategy::SpinWaitWaitStrategy, SpinWaitWaitStrategy()}
            };
        }
};
    
}

#endif