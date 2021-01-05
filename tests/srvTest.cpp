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

#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/details/basic_types.h>

#include "crane.h"
#include "Util.h"

#include "CraneSrv.h"

using namespace std;
using namespace NS_CRANE;

using namespace web;
using namespace web::http;
using namespace web::http::client;

struct HelloHandlerBasic : public Http::Handler {
    HTTP_PROTOTYPE(HelloHandlerBasic)

    void onRequest(const Http::Request & request, Http::ResponseWriter writer) override {
        cout << "------>Receive request with RURI: " << endl;
        cout << "------>" << request.address().host() << ":" << request.address().port() << endl;
        cout << "------>" << request.resource() << request.query().as_str() << endl;

        if (request.method() == Http::Method::Post) {
            cout << "------>Method: Post" << endl;
        }

        writer.send(Http::Code::Ok, "Hello, World!");
    }
};



#if 0
template <typename... Args>
Pistache::Rest::Route::Handler mybind(Pistache::Rest::Route::Result (*func)(Args...)) {
    return [=](const Rest::Request &request, Http::ResponseWriter response) {
        func(request, std::move(response));

        return Pistache::Rest::Route::Result::Ok;
    };
}
#endif

TEST(SRV, http_basic)
{
    cout << "Enter http_basic()" << endl;
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    Itf_CraneSrv* srv = nullptr;

    try {
        int argc = 1;
        char **argv = nullptr;
        pPluginFrame->init(argc, argv, CRANE_CRN);

        srv = dynamic_cast<Itf_CraneSrv*>(pPluginFrame->create("Itf_CraneSrv", "CraneSrv", "Crane HTTP Server."));

        srv->handler(Http::make_handler<HelloHandlerBasic>());

        srv->start(Itf_CraneSrv::ThreadMode::NEW_THREAD);
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    cout << "Client connect to Server" << endl;
    http_client_config config;
    config.set_validate_certificates(false);
    shared_ptr<http_client> client;
    client = make_shared<http_client>(U("http://127.0.0.1:8080/"), config);

    // Build request URI and start the request.
    uri_builder builder(U("/api/v1/img/dongyin"));

    auto response = client->request(methods::POST, builder.to_string()).get();

    ASSERT_TRUE(response.status_code() == status_codes::OK) << "Receive status code: " << response.status_code() << endl;

    srv->shutdown();
    pPluginFrame->destory(srv);
}

Pistache::Rest::Route::Result postProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    unused(request);
    cout << "Enter postProcess function." << endl;
    response.send(Pistache::Http::Code::Ok);
    return Pistache::Rest::Route::Result::Ok;
}

TEST(SRV, http_router_func)
{
    cout << "Enter http_basic()" << endl;
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    Itf_CraneSrv* srv = nullptr;

    try {
        int argc = 1;
        char **argv = nullptr;
        pPluginFrame->init(argc, argv, CRANE_CRN);

        srv = dynamic_cast<Itf_CraneSrv*>(pPluginFrame->create("Itf_CraneSrv", "CraneSrv", "Crane HTTP Server."));

        srv->router(HttpMethod::POST, string("/api/v1/img/dongyin"), Rest::Routes::bind(postProcess));
        srv->handler();
        srv->start(Itf_CraneSrv::ThreadMode::NEW_THREAD);
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    cout << "Client connect to Server" << endl;
    http_client_config config;
    config.set_validate_certificates(false);
    shared_ptr<http_client> client;
    client = make_shared<http_client>(U("http://127.0.0.1:8080/"), config);

    // Build request URI and start the request.
    uri_builder builder(U("/api/v1/img/dongyin"));

    auto response = client->request(methods::POST, builder.to_string()).get();
    ASSERT_TRUE(response.status_code() == status_codes::OK) << "Receive status code: " << response.status_code() << endl;

    srv->shutdown();
    pPluginFrame->destory(srv);
}

class Http_handler {
public:
    void postHandler(const Rest::Request& request, Http::ResponseWriter response)
    {
        unused(request);
        response.send(Http::Code::Ok, to_string(100));
    }

};

TEST(SRV, http_router_obj)
{
    cout << "Enter http_basic()" << endl;
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    Itf_CraneSrv* srv = nullptr;

    try {
        int argc = 1;
        char **argv = nullptr;
        pPluginFrame->init(argc, argv, CRANE_CRN);

        srv = dynamic_cast<Itf_CraneSrv*>(pPluginFrame->create("Itf_CraneSrv", "CraneSrv", "Crane HTTP Server."));

        Http_handler http_handler;
        srv->router(HttpMethod::POST, string("/api/v1/img/dongyin"), Rest::Routes::bind(&Http_handler::postHandler, &http_handler));
        srv->handler();
        
        srv->start(Itf_CraneSrv::ThreadMode::NEW_THREAD);
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    cout << "Client connect to Server" << endl;
    http_client_config config;
    config.set_validate_certificates(false);
    shared_ptr<http_client> client;
    client = make_shared<http_client>(U("http://127.0.0.1:8080/"), config);

    // Build request URI and start the request.
    uri_builder builder(U("/api/v1/img/dongyin"));

    auto response = client->request(methods::POST, builder.to_string()).get();
    ASSERT_TRUE(response.status_code() == status_codes::OK) << "Receive status code: " << response.status_code() << endl;

    srv->shutdown();
    pPluginFrame->destory(srv);
}

TEST(SRV, http_router_shared_ptr)
{
    cout << "Enter http_basic()" << endl;
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    Itf_CraneSrv* srv = nullptr;

    try {
        int argc = 1;
        char **argv = nullptr;
        pPluginFrame->init(argc, argv, CRANE_CRN);

        srv = dynamic_cast<Itf_CraneSrv*>(pPluginFrame->create("Itf_CraneSrv", "CraneSrv", "Crane HTTP Server."));

        shared_ptr<Http_handler> http_handler = make_shared<Http_handler>();
        srv->router(HttpMethod::POST, string("/api/v1/img/dongyin"), Rest::Routes::bind(&Http_handler::postHandler, http_handler));
        srv->handler();
        
        srv->start(Itf_CraneSrv::ThreadMode::NEW_THREAD);
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    cout << "Client connect to Server" << endl;
    http_client_config config;
    config.set_validate_certificates(false);
    shared_ptr<http_client> client;
    client = make_shared<http_client>(U("http://127.0.0.1:8080/"), config);

    // Build request URI and start the request.
    uri_builder builder(U("/api/v1/img/dongyin"));

    auto response = client->request(methods::POST, builder.to_string()).get();
    ASSERT_TRUE(response.status_code() == status_codes::OK) << "Receive status code: " << response.status_code() << endl;

    srv->shutdown();
    pPluginFrame->destory(srv);
}

TEST(SRV, http_init_by_code)
{
    cout << "Enter http_basic()" << endl;
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    Itf_CraneSrv* srv = nullptr;

    try {
        constexpr unsigned short port = 8000;
        int argc = 1;
        char **argv = nullptr;
        pPluginFrame->init(argc, argv, CRANE_CRN);

        srv = dynamic_cast<Itf_CraneSrv*>(pPluginFrame->create("Itf_CraneSrv", "CraneSrv", "Crane HTTP Server."));

        srv->create(port);
        srv->options()
            .threads(2)
            .maxRequestSize(40960)
            .maxResponseSize(40960)
            .threadsName("my_new_http_thread_name")
            .flags(Tcp::Options::ReuseAddr);
        srv->initialize();

        Http_handler http_handler;
        srv->router(HttpMethod::POST, string("/api/v1/img/dongyin"), Rest::Routes::bind(&Http_handler::postHandler, &http_handler));
        srv->handler();
        
        srv->start(Itf_CraneSrv::ThreadMode::NEW_THREAD);
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    cout << "Client connect to Server" << endl;
    http_client_config config;
    config.set_validate_certificates(false);
    shared_ptr<http_client> client;
    client = make_shared<http_client>(U("http://127.0.0.1:8000/"), config);

    // Build request URI and start the request.
    uri_builder builder(U("/api/v1/img/dongyin"));

    auto response = client->request(methods::POST, builder.to_string()).get();
    ASSERT_TRUE(response.status_code() == status_codes::OK) << "Receive status code: " << response.status_code() << endl;

    srv->shutdown();
    pPluginFrame->destory(srv);
}

TEST(SRV, https_basic)
{
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    int argc = 1;
    char **argv = nullptr;
    pPluginFrame->init(argc, argv, CRANE_CRN);

    Itf_CraneSrv* srv = dynamic_cast<Itf_CraneSrv*>(pPluginFrame->create("Itf_CraneSrv", "CraneSrv", "Crane HTTP Server."));

    srv->handler(Http::make_handler<HelloHandlerBasic>());
    srv->serverTLS("./certs/server.crt", "./certs/server.key");
    srv->clientTLS("./certs/rootCA.crt");
    srv->start(Itf_CraneSrv::ThreadMode::NEW_THREAD);

    cout<<"Client connect to Server"<<endl;
    
    http_client_config config;
    config.set_ssl_context_callback( [&](boost::asio::ssl::context& ctx) {
      ctx.set_options(boost::asio::ssl::context::default_workarounds);
      boost::system::error_code ec;
      ctx.use_certificate_file("./certs/client.crt", boost::asio::ssl::context_base::file_format::pem, ec);
      cout<<"error code: "<<ec.message()<<endl;
      ctx.use_private_key_file("./certs/client.key", boost::asio::ssl::context_base::file_format::pem);
      ctx.load_verify_file("./certs/rootCA.crt");
    });
    config.set_validate_certificates(false);

    shared_ptr<http_client> client;
    client = make_shared<http_client>(U("https://127.0.0.1:8080/"), config);

    // Build request URI and start the request.
    uri_builder builder(U("/api/v1/img/dongyin"));

    auto response = client->request(methods::POST, builder.to_string()).get();
    ASSERT_TRUE(response.status_code() == status_codes::OK) << "Receive status code: " << response.status_code() << endl;

    srv->shutdown();
    pPluginFrame->destory(srv);
}

