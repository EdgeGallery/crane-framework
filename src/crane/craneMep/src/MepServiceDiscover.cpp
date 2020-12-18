/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-02 12:16:39
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-07-16 12:22:31
 */ 
#include "MepServiceDiscover.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace NS_CRANE {

    MepClazzType MepServiceDiscover::ClazzType() {
        return MepClazzType::ServiceDiscover;
    }
    
    void MepServiceDiscover::doInteraction() {
        
        _result = CRANE_FAIL;

        //vector<Async::Promise<Http::Response>> responses;
        //atomic<size_t> completedRequests(0);
        //atomic<size_t> failedRequests(0);

        _commpleted = 0;

        //完成和MEP APIGW的服务发现交互过程
        if (true == _services.empty()) {
            client_ = make_shared<http_client>(U(HTTPS + _endpoint), clientConfig_);
            LOG_DEBUG("Connect to %s", (HTTPS + _endpoint).c_str());
            uri_builder builder = uri_builder();
            builder.append(U("/mep/mec_service_mgmt/v1/services"));

            auto rsp = client_->request(methods::GET, builder.to_string()).get();

            const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();
            rspAllServiceProcess(rspBody, rsp.status_code());

            _result = CRANE_SUCC; 

        } else {
            for (auto s : _services) {
                //string getServiesUrl = "https://" + _endpoint  + "/services?/" + "/serName=" + s;

                client_ = make_shared<http_client>(U(HTTPS + _endpoint), clientConfig_);
                LOG_DEBUG("Connect to %s", (HTTPS + _endpoint).c_str());

                uri_builder builder = uri_builder();
                builder.append(U("/mep/mec_service_mgmt/v1/services"));
                builder.append_query(U("serName"), U(s));
                LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

                //http_request req(methods::GET);
                auto rsp = client_->request(methods::GET, builder.to_string()).get();

                const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();
                rspSingleServiceProcess(rspBody, rsp.status_code());
            }

            if (_commpleted == _services.size()) {
                _result = CRANE_SUCC;
            } else {
                _result = CRANE_FAIL;
            }
        }
        
        _mediator->MepServiceResult(shared_from_this());
        return;
/*
            string rspBody;
            Http::Code statusCode;

            auto rsp = _mediator->client.get(getServiesUrl).send();
            rsp.then(
                [&rspBody, &statusCode, &completedRequests, this](Pistache::Http::Response response) {
                    ++completedRequests;
                    fprintf(stdout, "Response code[ %u ]\n", static_cast<unsigned>(response.code()));
                    rspBody = response.body();
                    statusCode = response.code();
                    if (!rspBody.empty()) {
                        fprintf(stdout, "Response body[ %s ]\n", rspBody.c_str());
                        rspProcess(rspBody, statusCode);
                    }
                },
                [&](std::exception_ptr exc) {
                    ++failedRequests;
                    fprintf(stderr, "Occur exeception when process the discovering service. \n");
                    PrintException excPrinter;
                    excPrinter(exc);
                }
            );
            responses.push_back(move(rsp));
        }

        auto sync = Async::whenAll(responses.begin(), responses.end());
        Async::Barrier<vector<Http::Response>> barrier(sync);
        barrier.wait_for(std::chrono::seconds(5));
        
        if (completedRequests.load() == _services.size()) {
            //If all request has received response, whether it's success or failure, the execution at this stage is considered successfull.
            //MepMediator will process the result and decide what to do next.
            _result = CRANE_SUCC;
        } else {
            _result = CRANE_FAIL;
        }
        _mediator->MepServiceResult(shared_from_this());
        return;
*/        
    }

    void MepServiceDiscover::rspSingleServiceProcess(const string& rspBody, const web::http::status_code statusCode) {

        LOG_DEBUG("Status code: %u", statusCode);
        LOG_DEBUG("Response Json Body: %s", rspBody.c_str());

        do {
            if (statusCode != web::http::status_codes::OK) {
                //解析body中的失败信息描述；
                ErrorRspBody errBody = parseRspErrorBody(rspBody);
                LOG_ERROR("Response statusCode[ %u ] with detail info [ %s ]", 
                            static_cast<unsigned>(statusCode), errBody.detail.c_str());

                _result = CRANE_FAIL;
                break;
            }

            Document d;
            d.Parse(rspBody.c_str());
            list<string> h;
            Value* v; 

            //fatch "serId"
            h = {"serInstanceId"};
            v = Util::fatchJsonValue(d, h);
            if (v == nullptr) {return;}
            string id = string(v->GetString());

            //fatch "serName"
            h = {"serName"};
            v = Util::fatchJsonValue(d, h);
            if (v == nullptr) {return;}
            string name = string(v->GetString());

            //fatch "address"
            MepService::Endpoints endpoints;
            h = {"transportInfo", "endpoint", "addresses"};
            v = Util::fatchJsonValue(d, h);
            if (v == nullptr) {return;}
            if (!v->IsArray()) {return;}
            for (auto itr = v->Begin(); itr != v->End(); ++itr ) {
                string address;
                if ((itr->HasMember("host")) && (itr->HasMember("port"))) {
                    address = string(itr->FindMember("host")->value.GetString()) + ":"
                        + string(itr->FindMember("port")->value.GetString());
                    endpoints.push_back(address);
                }
            }
            shared_ptr<MepService> mepService = make_shared<MepService>(id, name);
            mepService->endpoints = endpoints;

            _servicesInfo.insert(make_pair(name, mepService));

            auto itr = _servicesInfo.find(name);
            if (itr != _servicesInfo.end()) {
                LOG_DEBUG("Discover service info: ID[%s], Name[%s]", itr->second->srvId.c_str(), itr->second->srvName.c_str());
                for (auto ep : itr->second->endpoints) {
                    LOG_DEBUG("Discover service endpoint info: Address[%s]", ep.c_str());
                }
            }

            ++_commpleted;
        } while(0);

        return;
    }

    void MepServiceDiscover::rspAllServiceProcess(const string& rspBody, const web::http::status_code statusCode) {

        LOG_DEBUG("Status code: %u", statusCode);
        LOG_DEBUG("Response Json Body: %s", rspBody.c_str());

        do {
            if (statusCode != web::http::status_codes::OK) {
                //解析body中的失败信息描述；
                ErrorRspBody errBody = parseRspErrorBody(rspBody);
                LOG_ERROR("Response statusCode[ %u ] with detail info [ %s ]", 
                            static_cast<unsigned>(statusCode), errBody.detail.c_str());

                _result = CRANE_FAIL;
                break;
            }

            Document d;
            d.Parse(rspBody.c_str());
            list<std::string> h;

            if (!d.IsArray()) {break;}

            for (rapidjson::Value::ValueIterator itr = d.Begin(); itr!= d.End(); ++itr) {

                //fatch "serId"
                h = {"serInstanceId"};
                rapidjson::Value* v = Util::fatchJsonValue((*itr), h);
                if (nullptr == v) {break;}
                string id = string(v->GetString());

                //fatch "serName"
                h = {"serName"};
                v = Util::fatchJsonValue(*itr, h);
                if (v == nullptr) {break;}
                string name = string(v->GetString());

                //fatch "address"
                MepService::Endpoints endpoints;
                h = {"transportInfo", "endpoint", "addresses"};
                v = Util::fatchJsonValue(*itr, h);
                if (v == nullptr) {break;}
                if (!v->IsArray()) {break;}
                for (auto itr = v->Begin(); itr != v->End(); ++itr ) {
                    string address;
                    if ((itr->HasMember("host")) && (itr->HasMember("port"))) {
                        address = string(itr->FindMember("host")->value.GetString()) + ":"
                            + to_string(itr->FindMember("port")->value.GetInt());
                        endpoints.push_back(address);
                    }
                }

                shared_ptr<MepService> mepService = make_shared<MepService>(id, name);
                mepService->endpoints = endpoints;

                _servicesInfo.insert(make_pair(name, mepService));

                auto i = _servicesInfo.find(name);
                if (i != _servicesInfo.end()) {
                    LOG_DEBUG("Discover service info: ID[%s], Name[%s]", i->second->srvId.c_str(), i->second->srvName.c_str());
                    for (auto ep : i->second->endpoints) {
                        LOG_DEBUG("Discover service endpoint info: Address[%s]", ep.c_str());
                    }
                }
            }


        } while(0);

        return;
    }

    void  MepServiceDiscover::abnormity() {
        _result = CRANE_FAIL;
        _mediator->MepServiceResult(shared_from_this());
    }
}