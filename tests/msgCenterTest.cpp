/*
 * @Descripttion: Test program of CraneMsgCenter system plugin.
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-10 15:36:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-10 18:18:51
 */

#include <thread>

#include "gtest/gtest.h"

#include "crane.h"
#include "Util.h"

#include "CraneMsgCenterPlugin.h"

using namespace std;
using namespace NS_CRANE;

//A example of CraneMsgImpi's subclass, which should define the data type of msg and implement the public method.
class TestMsg : public CraneMsgImpi, public enable_shared_from_this<TestMsg> {
    public:
        TestMsg(CraneTopic topic, string data) : CraneMsgImpi(topic), _data(data) {
            cout<<"TestMsg()"<<endl;
        }

        /**
         * @Descripttion: Implement the pure virtual method, which publish the data to CraneMsgCenter.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAIL
         */
        unsigned publish() override {
            cout<<"publish()"<<endl;
            msgCenter()->publish(topic(), dynamic_pointer_cast<CraneMsg>(shared_from_this()));
            return CRANE_SUCC;
        }
    
        const string& data() const { return _data; }

        void setData(string& data) { _data = data; }
        
    private:
        string                  _data;
};

//A example of CraneObserverImpi subclass, which should implement the template method such as doUpdata() or doNotify().
class TestObserver : public CraneObserverImpi {
    public:
        TestObserver() {
            cout<<"TestObserver()"<<endl;
            thread processThread(&TestObserver::_process, this);
            processThread.detach();
        }

        /**
         * @Descripttion: For sync msg, CraneMsgCenter will invoke this method to update new data.
         *                The call stack is invoke by the msg publisher.
         * @Param:  topic:  topic of data.   
         * @Param:  data:   pointer of data which is wrapped by shared_ptr.   
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        unsigned doUpdate(const CraneTopic& topic, shared_ptr<const CraneMsg> data) override {
            cout<<"doUpdate()"<<endl;
            cout<<"Topic.name: "<<topic.name()<<"Topic.type: "<<static_cast<unsigned>(topic.type())<<endl;

            shared_ptr<const TestMsg> msg = dynamic_pointer_cast<const TestMsg>(data);
            cout<<"Data: { "<<msg->data()<<" }"<<endl;
            return CRANE_SUCC;
        }

        /**
         * @Descripttion: For async msg, CraneMsgCenter will invoke this method to notify the observer a data has arrrived.
         *                This call stack is in CraneMsgCenter's thread, so observer should not do too much work in this function.
         *                Fetch and process data process should do in observer's thread.
         * @Param: 
         * @Return: 
         */        
        unsigned doNotify(const CraneTopic& topic, shared_ptr<const CraneMsg> data) override {
            cout<<"doNotify()"<<endl;
            cout<<"Topic.name: "<<topic.name()<<"Topic.type: "<<static_cast<unsigned>(topic.type())<<endl;

            shared_ptr<const TestMsg> msg = dynamic_pointer_cast<const TestMsg>(data);
            _key = msg->key();

            return CRANE_SUCC;
        }        
        
    private:
        size_t _key = 0;

        void _process() {
            while (true) {
                if (_key != 0) {
                    const shared_ptr<CraneMsg>& completeData = msgCenter()->pull(_key);
                    cout<<"Fetch data from datapool with key: { "<<dynamic_pointer_cast<TestMsg>(completeData)->data()<<" }"<<endl;
                    _key = 0;
                }
                this_thread::yield();
            }
        }
};

TEST(MsgCenter, MsgCenterAsync) {
    cout<<"Enter MsgCenterAsync()"<<endl; 
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    int argc = 1;
    char **argv = nullptr;
    pPluginFrame->init(argc, argv, CRANE_CRN);

    //Create the sync and async topic.
    CraneTopic sync_topic("dongyin", CraneTopic::Type::SYNC);
    CraneTopic async_topic("dongkezhen", CraneTopic::Type::ASYNC);

    //Create two observers.
    shared_ptr<TestObserver> observer1 = make_shared<TestObserver>(); cout<<"shared_ptr observer1 addr : "<<&observer1<<endl;
    shared_ptr<TestObserver> observer2 = make_shared<TestObserver>(); cout<<"shared_ptr observer2 addr: "<<&observer2<<endl;

    //Subscriber the topics.
    observer1->subscribe(sync_topic);
    observer1->subscribe(async_topic);

    observer2->subscribe(sync_topic);
    observer2->subscribe(async_topic);

    //Create sync message, and publish it.
    shared_ptr<TestMsg> sync_msg = make_shared<TestMsg>(sync_topic, "I am dongyin.");
    sync_msg->publish();

    //Create async message, and publish it.
    shared_ptr<TestMsg> async_msg = make_shared<TestMsg>(async_topic, "I am dongkezhen.");
    async_msg->publish();


    this_thread::sleep_for(chrono::seconds(1));


    //Observer2 unsubscribe the topics.
    observer2->unsubscribe(sync_topic);
    observer2->unsubscribe(async_topic);

    //publish the sync and async message again.
    sync_msg = make_shared<TestMsg>(sync_topic, "I am dongyin, I come back again.");
    sync_msg->publish();
    async_msg = make_shared<TestMsg>(async_topic, "I am dongkezhen, I come back again.");
    async_msg->publish();


    this_thread::sleep_for(chrono::seconds(3));
}