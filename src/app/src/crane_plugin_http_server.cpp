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

#include <thread>

#include "crane.h"

#include "CraneSrv.h"

using namespace std;
using namespace NS_CRANE;

struct HelloHandlerBasic : public Http::Handler
{
    HTTP_PROTOTYPE(HelloHandlerBasic)

    void onRequest(const Http::Request & request, Http::ResponseWriter writer) override
    {
        cout << "------>Receive request with RURI: " << endl;
        cout << "------>" << request.address().host() << ":" << request.address().port() << endl;
        cout << "------>" << request.resource() << request.query().as_str() << endl;

        if (request.method() == Http::Method::Post) {
            cout << "------>Method: Post" << endl;
        }
        writer.send(Http::Code::Ok, "Hello, World!");
    }
};

int main(int argc, char** argv)
{
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
