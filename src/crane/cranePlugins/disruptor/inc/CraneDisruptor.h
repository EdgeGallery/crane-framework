/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-03-31 19:38:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-04-02 17:51:23
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
 * @Date: 2021-03-31 19:38:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-31 19:38:15
 */

#ifndef __PLUGIN_CRANE_DISRUPTOR_H__
#define __PLUGIN_CRANE_DISRUPTOR_H__

#include "ICraneDisruptor.h"

using namespace std;

namespace NS_CRANE {

struct DisruptorEvent
{
    string name;
    uint32_t age;
};

class CraneDisruptor : public Itf_CraneDisruptor<DisruptorEvent> {
public:
    unsigned int init() override;
    unsigned start() override;
    unsigned stop() noexcept override;

    unsigned swap_up(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) override;
    unsigned swap_down(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) override;     
    
    void initialize(const Options& ops) override;
    void startUp() override;
    void closeUp() override;
    //void handler(Disruptor::IEventHandler<DisruptorEvent>& d) override;
    void handler(EventHandlers& ds) override;
    void publish(DisruptorEvent e) override;
    virtual ~CraneDisruptor() { cout << "~CraneDisruptor()" << endl; }
    
    private:
        function<DisruptorEvent()>              _eventFactory;
        shared_ptr<Disruptor::ITaskScheduler>   _taskScheduler;
        shared_ptr<Disruptor::IWaitStrategy>    _waitStrategy;
        shared_ptr<Disruptor::disruptor<DisruptorEvent>>     _disruptor;
        uint32_t                                _threadNum;
};
    
}

#endif