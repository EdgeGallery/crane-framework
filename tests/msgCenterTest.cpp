/*
 * @Descripttion: Test program of CraneMsgCenter system plugin.
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-10 15:36:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-12 20:00:51
 */

#include <thread>

#include "gtest/gtest.h"

#include "crane.h"

#include "CraneMsgCenterPlugin.h"

using namespace std;
using namespace NS_CRANE;

//A example of CraneMsgImpi's subclass, which should define the data type of msg and implement the public method.
#if 0
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
#endif

//A example of CraneObserverImpi subclass, which should implement the template method such as doUpdata() or doNotify().
class TestObserver : public CraneObserverImpi {
    public:
        TestObserver() {
            cout<<"TestObserver()"<<endl;
            thread processThread(&TestObserver::_process, this);
            processThread.join();
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

            shared_ptr<const CraneMsgImpi<unsigned long>> msg = dynamic_pointer_cast<const CraneMsgImpi<unsigned long>>(data);
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

            shared_ptr<const CraneMsgImpi<unsigned long>> msg = dynamic_pointer_cast<const CraneMsgImpi<unsigned long>>(data);
            _key = msg->key();

            return CRANE_SUCC;
        }        
        
    private:
        //constexpr unsigned long shreshold = 10000;
        size_t _key = 0;

        void _process() {
            unsigned long count = 0;
            while (true) {
                if (count > 10000) break;
                if (_key != 0) {
                    const shared_ptr<CraneMsg>& completeData = msgCenter()->pull(_key);
                    cout<<"Fetch data from datapool with key: { "<<dynamic_pointer_cast<const CraneMsgImpi<unsigned long>>(completeData)->data()<<" }"<<endl;
                    _key = 0;
                }

                ++count;
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
    //shared_ptr<TestMsg> sync_msg = make_shared<TestMsg>(sync_topic, "I am dongyin.");
    shared_ptr<CraneMsgImpi<unsigned long>> sync_msg = make_shared<CraneMsgImpi<unsigned long>>(sync_topic);
    unsigned long sync_data{1};
    sync_msg->data(sync_data);
    sync_msg->publish();

    //Create async message, and publish it.
    //shared_ptr<TestMsg> async_msg = make_shared<TestMsg>(async_topic, "I am dongkezhen.");
    shared_ptr<CraneMsgImpi<unsigned long>> async_msg = make_shared<CraneMsgImpi<unsigned long>>(async_topic);
    unsigned long async_data{2};
    async_msg->data(async_data);
    async_msg->publish();


    //this_thread::sleep_for(chrono::seconds(1));


    //Observer2 unsubscribe the topics.
    observer2->unsubscribe(sync_topic);
    observer2->unsubscribe(async_topic);

    //publish the sync and async message again.
    //sync_msg = make_shared<TestMsg>(sync_topic, "I am dongyin, I come back again.");
    sync_msg = make_shared<CraneMsgImpi<unsigned long>>(sync_topic);
    sync_data = 3;
    sync_msg->data(sync_data);
    sync_msg->publish();
    //async_msg = make_shared<TestMsg>(async_topic, "I am dongkezhen, I come back again.");
    async_msg = make_shared<CraneMsgImpi<unsigned long>>(async_topic);
    async_data = 4;
    async_msg->data(async_data);
    async_msg->publish();


    //this_thread::sleep_for(chrono::seconds(3));
}

std::atomic<bool> g_finished{false};

class Data final {
    public:
        inline void data(const string& d) { _str = d; }
        inline const string& data(void) { return _str; }
    private:
        string      _str;
};

class TestObserverConcurrent : public CraneObserverImpi {
    public:
        explicit TestObserverConcurrent(const string& name) : _name(name) {
            LOG_DEBUG("TestObserverConcurrent( %s )", _name.c_str());
            //thread fetchThread([=](int i) ->void { cout<<"hello world! "<<str<<i<<endl; }, 43);
            thread fetchThread([this]() ->void {
                unsigned long count = 0;
                while (true) {
                    if (g_finished == true) { cout<<_name<<" finished the fetch process"<<endl; return; }
                    if (count > 10000000) { cout<<"count: "<<count<<endl; return; }
                    if (!_keys.empty()) {

                        _mtx.lock();
                        const size_t k = _keys.front();
                        _keys.pop();
                        _mtx.unlock();

                        auto completeData = msgCenter()->pull(k);
                        Data d = dynamic_pointer_cast<const CraneMsgImpi<Data>>(completeData)->data();
                        //Data d = completeData->data();
                        LOG_DEBUG("%s fetch the data { %s }", _name.c_str(), d.data().c_str());
                    }
                    ++count;
                    this_thread::yield();
                }});
            _thread = std::move(fetchThread);
        }

        /**
         * @Descripttion: For async msg, CraneMsgCenter will invoke this method to notify the observer a data has arrrived.
         *                This call stack is in CraneMsgCenter's thread, so observer should not do too much work in this function.
         *                Fetch and process data process should do in observer's thread.
         * @Param: 
         * @Return: 
         */        
        unsigned doNotify(const CraneTopic& topic, shared_ptr<const CraneMsg> data) override {
            shared_ptr<const CraneMsgImpi<Data>> msg = dynamic_pointer_cast<const CraneMsgImpi<Data>>(data);
            LOG_DEBUG("Topic.name { %s } Topic.type { %u } key { %u }", 
                        topic.name().c_str(), static_cast<std::underlying_type<CraneTopic::Type>::type>(topic.type()), msg->key());

            _mtx.lock();
            _keys.push(msg->key());
            _mtx.unlock();
            
            return CRANE_SUCC;
        }     

        inline thread& getThread() { return _thread; }   
        
    private:
        queue<size_t>        _keys;
        mutex               _mtx;
        string              _name;
        thread              _thread;
};

TEST(MsgCenter, MsgCenterConcurrent) {
    cout<<"Enter MsgCenterConcurrent()"<<endl; 
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    int argc = 1;
    char **argv = nullptr;
    pPluginFrame->init(argc, argv, CRANE_CRN);

    //Create the sync and async topic.
    CraneTopic concurrent_topic("concurrent", CraneTopic::Type::ASYNC);

    //Create two observers.
    shared_ptr<TestObserverConcurrent> observer_1 = make_shared<TestObserverConcurrent>("observer_1");
    shared_ptr<TestObserverConcurrent> observer_2 = make_shared<TestObserverConcurrent>("observer_2");

    observer_1->subscribe(concurrent_topic);
    observer_2->subscribe(concurrent_topic);

    thread feedThread([concurrent_topic]() ->void {
        unsigned long count = 0;
        while (true) {
            if (count > 10) break;
            shared_ptr<CraneMsgImpi<Data>> concurrent_msg = make_shared<CraneMsgImpi<Data>>(concurrent_topic);
            Data concurrent_data;
            string str = "Dongyin feed " + to_string(count) + " times."; 
            concurrent_data.data(str);
            concurrent_msg->data(concurrent_data);
            concurrent_msg->publish();

            ++count;
            this_thread::yield();
        }
        cout<<"============Finished feed process.============"<<endl;
        g_finished = true;
    });
    if (feedThread.joinable()) feedThread.join();
    if (observer_1->getThread().joinable()) observer_1->getThread().join();
    if (observer_2->getThread().joinable()) observer_2->getThread().join();
    cout<<"==========Finished=========="<<endl;
}