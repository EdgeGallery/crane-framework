/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-04-02 09:39:41
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-04-02 18:04:21
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
#include "gtest/gtest.h"

#include "crane.h"

#include "playerInterface.h"

#include "CraneDisruptor.h"

using namespace std;
using namespace NS_CRANE;

class DisrutporTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        pluginFrame = AbstractPluginFrame::getPluginFrame();
        pluginFrame->init(1, nullptr, CRANE_CRN);
    }

    virtual void TearDown() {
        AbstractPluginFrame::relPluginFrame();
    }
    
    AbstractPluginFrame* pluginFrame = nullptr;
};

class EventHandler : public Disruptor::IEventHandler<DisruptorEvent>, 
                    public Disruptor::ILifecycleAware {
public:
    explicit EventHandler(const string& handlerName, int expectedProcessed) : 
        _handlerName(handlerName),
        _expectProcessed(expectedProcessed) {}
    void onEvent(DisruptorEvent& event, int64_t seq, bool b) override {
        //cout << _handlerName << ", Age: " << event.age << endl;
        _sumOfAge += event.age;

        if (++_actuallyProcessed == _expectProcessed) {
            _allDone.notify_all();
        }
    }

    void waitEndOfProcessing() {
        unique_lock<decltype(_mtx)> lk(_mtx);
        _allDone.wait_for(lk, std::chrono::seconds(1));
    }

    void onStart() override {
        cout << "onStart()" << endl;
    }

    void onShutdown() override {
        cout << "onShutdown()" << endl;
    }

    uint32_t sumOfAge() const { return _sumOfAge; }

private:
    string      _handlerName;
    uint32_t    _expectProcessed;
    uint32_t    _actuallyProcessed{0};
    mutex   _mtx;
    condition_variable  _allDone;
    uint32_t    _expectedAge{0};
    uint32_t    _sumOfAge{0};
};

TEST_F(DisrutporTest, base) {
    cout << endl << endl;
    cout <<"Enter DisruptorTest base()" << endl;

    uint32_t expectedNumOfEvents = 10000;

    //*** Create disruptor instance.
    string disruptor_id{"disruptor_1"};
    shared_ptr<Itf_CraneDisruptor<DisruptorEvent>> disruptor = 
        dynamic_pointer_cast<Itf_CraneDisruptor<DisruptorEvent>>(pluginFrame->create(
                                                            "Itf_CraneDisruptor", 
                                                            "CraneDisruptor",
                                                            disruptor_id, 
                                                            ""));
    Itf_CraneDisruptor<DisruptorEvent>::Options ops{};
    disruptor->initialize(ops);
    
    auto eventHandler_1 = make_shared<EventHandler>("handler_1", expectedNumOfEvents);
    auto eventHandler_2 = make_shared<EventHandler>("handler_2", expectedNumOfEvents);
    Itf_CraneDisruptor<DisruptorEvent>::EventHandlers handlers{eventHandler_1, eventHandler_2};
    disruptor->handler(handlers);

    disruptor->startUp();
    
    uint32_t sumOfAge{0};
    for(uint32_t i = 0; i<expectedNumOfEvents; ++i) {
        disruptor->publish(DisruptorEvent{"dongyin", i});
        sumOfAge += i;
    }

    eventHandler_1->waitEndOfProcessing();
    eventHandler_2->waitEndOfProcessing();

    ASSERT_EQ(sumOfAge, eventHandler_1->sumOfAge()); 
    ASSERT_EQ(sumOfAge, eventHandler_2->sumOfAge()); 

    disruptor->closeUp();

    pluginFrame->release(std::move(disruptor));
        
}

TEST_F(DisrutporTest, RoundRobinThreadAffinedTaskScheduler) {
    cout << endl << endl;
    cout <<"Enter DisruptorTest base2()" << endl;

    uint32_t expectedNumOfEvents = 10000;

    //*** Create disruptor instance.
    string disruptor_id{"disruptor_1"};
    shared_ptr<Itf_CraneDisruptor<DisruptorEvent>> disruptor = 
        dynamic_pointer_cast<Itf_CraneDisruptor<DisruptorEvent>>(pluginFrame->create(
                                                            "Itf_CraneDisruptor", 
                                                            "CraneDisruptor",
                                                            disruptor_id, 
                                                            ""));
    Itf_CraneDisruptor<DisruptorEvent>::Options ops{};
    ops.taskSchedulerPolicy(Itf_CraneDisruptor<DisruptorEvent>::TaskSchedulerPolicy::RoundRobinThreadAffinedTaskScheduler);
    
    disruptor->initialize(ops);

    auto eventHandler_1 = make_shared<EventHandler>("hangler_1" ,expectedNumOfEvents);
    auto eventHandler_2 = make_shared<EventHandler>("handler_2", expectedNumOfEvents);
    Itf_CraneDisruptor<DisruptorEvent>::EventHandlers handlers{eventHandler_1,
                                                                eventHandler_2};
    disruptor->handler(handlers);

    disruptor->startUp();

    uint32_t sumOfAge{0};
    for(uint32_t i = 0; i<expectedNumOfEvents; ++i) {
        disruptor->publish(DisruptorEvent{"dongyin", i});
        sumOfAge += i;
    }
    eventHandler_1->waitEndOfProcessing();
    eventHandler_2->waitEndOfProcessing();
    ASSERT_EQ(sumOfAge, eventHandler_1->sumOfAge());
    ASSERT_EQ(sumOfAge, eventHandler_2->sumOfAge());

    disruptor->closeUp();

    pluginFrame->release(std::move(disruptor));
}