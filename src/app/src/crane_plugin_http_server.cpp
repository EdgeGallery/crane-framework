/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-23 10:44:45
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-04 17:32:43
 */
#include <thread>

#include "crane.h"

#include "CraneSrv.h"

using namespace std;
using namespace NS_CRANE;

struct HelloHandlerBasic : public Http::Handler {
  HTTP_PROTOTYPE(HelloHandlerBasic)

  void onRequest(const Http::Request & request, Http::ResponseWriter writer) override {
    cout<<"------>Receive request with RURI: "<<endl;
    cout<<"------>"<<request.address().host()<<":"<<request.address().port()<<request.resource()<<request.query().as_str()<<endl;

    if (request.method() == Http::Method::Post) {
      cout<<"------>Method: Post"<<endl;
    }
    /*
    thread worker([](Http::ResponseWriter w) { 
        w.send(Http::Code::Ok, "Hello, World!");
     }, std::move(writer));
     worker.detach();
     */
    writer.send(Http::Code::Ok, "Hello, World!");
  }
};

int main(int argc, char** argv) { 
    //*** Create a singleton object of plugin frame.
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();

    //*** Initialize Crane and Gstreamer system.
    pPluginFrame->init(argc, argv, CRANE_CRN);

    //*** Create "SingletonImpl" instance of crane singtonImpl plugin.
    Itf_CraneSrv* server = dynamic_cast<Itf_CraneSrv*>(
        pPluginFrame->create("Itf_CraneSrv", "CraneSrv", "Crane HTTP Server."));

    server->handler(Http::make_handler<HelloHandlerBasic>());

    server->start(Itf_CraneSrv::ThreadMode::MAIN_THREAD);

    return 0;
}