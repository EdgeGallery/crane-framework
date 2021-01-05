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

#include "MepServiceDiscover.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace NS_CRANE {
    const string MepServiceDiscover::_SERVICE_PATH = "/mep/mec_service_mgmt/v1/services";
    const string  MepServiceDiscover::_PARA_NAME = "serName"; 

    MepClazzType MepServiceDiscover::ClazzType() {
        return MepClazzType::ServiceDiscover;
    }
    
    void MepServiceDiscover::doInteraction(MepProduceType type) {
        result_ = { type, CRANE_FAIL };
        _commpleted = 0;

        //完成和MEP APIGW的服务发现交互过程
        if (true == _services.empty()) {
            LOG_DEBUG("Discover all services in mep at a time.");

            client_ = make_shared<http_client>(U(HTTPS + endpoint_), clientConfig_);
            LOG_DEBUG("Connect to %s", (HTTPS + endpoint_).c_str());
            uri_builder builder = uri_builder();
            builder.append(U(_SERVICE_PATH));
            LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

            auto rsp = client_->request(methods::GET, builder.to_string()).get();

            const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();
            rspAllServiceProcess(rspBody, rsp.status_code());
        } else {
            for (auto s : _services) {
                LOG_DEBUG("Discover services in mep in sequence.");

                client_ = make_shared<http_client>(U(HTTPS + endpoint_), clientConfig_);
                LOG_DEBUG("Connect to %s", (HTTPS + endpoint_).c_str());

                uri_builder builder = uri_builder();
                builder.append(U(_SERVICE_PATH));
                builder.append_query(U(_PARA_NAME), U(s));
                LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

                auto rsp = client_->request(methods::GET, builder.to_string()).get();

                const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();
                rspSingleServiceProcess(rspBody, rsp.status_code());
            }

            if (_commpleted == _services.size()) {
                //result_ = CRANE_SUCC;
                result_ = { type, CRANE_SUCC };
            } else {
                result_ = { type, CRANE_FAIL };
                //result_ = CRANE_FAIL;
            }
        }
        
        mediator_->MepServiceResult(shared_from_this());
        return;
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

                //result_ = CRANE_FAIL;
                result_ = {MepProduceType::DOALL, CRANE_FAIL};
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

                //result_ = CRANE_FAIL;
                result_ = {MepProduceType::DOALL, CRANE_FAIL};
                break;
            }

            Document d;
            d.Parse(rspBody.c_str());
            if (d.HasParseError()) {
                LOG_ERROR("Parse error: { %d : %lu } %s \n", 
                d.GetParseError(), 
                d.GetErrorOffset(),  
                (char*)rapidjson::GetParseErrorFunc(d.GetParseError()));
                return;    
            }

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
        //result_ = CRANE_FAIL;
        result_ = {MepProduceType::DOALL, CRANE_FAIL};
        mediator_->MepServiceResult(shared_from_this());
    }
}
