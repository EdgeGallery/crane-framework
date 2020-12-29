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

#include "MepServiceSubscriber.h"

namespace NS_CRANE {

    const string MepServiceSubscriber::_SERVICE_PATH = "/mep/mec_service_mgmt/v1/applications/";
    const string MepServiceSubscriber::_SERVICE_RESOURCE = "/subscriptions";
    const string MepServiceSubscriber::_CALLBACK_REF = "SubscriberNotify";
    
    const string MepServiceSubscriber::_SERVICE_SUBSCRIBE_BODY = "{\
        \"subscriptionType\": \"SerAvailabilityNotificationSubscription\",\
        \"callbackReference\": \"string\",\
        \"filteringCriteria\": {\
          \"serNames\": [\
            \"ExampleService\"\
          ],\
          \"states\": [\
            \"ACTIVE\"\
          ],\
          \"isLocal\": true\
          }\
    }";

    MepClazzType MepServiceSubscriber::ClazzType() {
        return MepClazzType::ServiceSubscirber;
    }
    
    void MepServiceSubscriber::doInteraction(MepProduceType type) { 
        result_ = {type, CRANE_FAIL};

        //完成和MEP APIGW的服务订阅交互过程
        //std::string postServiceSubscirberUrl = "http://" + _endpoint  + "/applications/" + _appInstanceId + "/subscriptions";

        //vector<Async::Promise<Http::Response>> responses;
        //atomic<size_t> completedRequests(0);
        //atomic<size_t> failedRequests(0);

        _commpleted = 0;

        for(string s : _services) {
            Document d;
            d.Parse(_SERVICE_SUBSCRIBE_BODY.c_str());
            if (d.HasParseError()) {
                LOG_ERROR("Parse error: { %d : %lu } %s \n", 
                d.GetParseError(), 
                d.GetErrorOffset(),  
                (char*)rapidjson::GetParseErrorFunc(d.GetParseError()));
                return;
            }            
            auto& a = d.GetAllocator();

            list<string> h = {"callbackReference"};
            Value* v = Util::fatchJsonValue(d, h);
            if (v == nullptr) {return;}
            string newCallbackRef = getCallbackRef(); 
            v->SetString(newCallbackRef.c_str(), static_cast<SizeType>(newCallbackRef.size()), a);

            //将vector中服务名称添加到数组中
            h = {"filteringCriteria","serNames"};
            v = Util::cleanJsonArray(d, h);
            Value sn(kStringType);
            sn.SetString(s.c_str(), static_cast<SizeType>(s.size()),a);
            v->PushBack(sn, a); 

            h = {"filteringCriteria", "states"};
            v = Util::cleanJsonArray(d, h);
            v->PushBack("ACTIVE", a);
            v->PushBack("INACTIVE", a);
    
            Util::printJson(d);

            client_ = make_shared<http_client>(U(HTTPS + endpoint_), clientConfig_);
            LOG_DEBUG("Connect to %s", (HTTPS + endpoint_).c_str());

            uri_builder builder = uri_builder();
            builder.append(_SERVICE_PATH + appInstanceId_ + _SERVICE_RESOURCE);
            LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

            auto rsp = client_->request(methods::POST, builder.to_string(), Util::Json2String(d)).get();

            const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();
            rspProcess(rsp.status_code(), rsp.headers(), rspBody);
        }
        
        if (_commpleted == _services.size()) {
            result_ = {MepProduceType::DOALL, CRANE_SUCC};
        } else {
            result_ = {MepProduceType::DOALL, CRANE_FAIL};
        }
        mediator_->MepServiceResult(shared_from_this());
        return;
 
    }

    void MepServiceSubscriber::DoServiceNotify(const Rest::Request& request, Http::ResponseWriter response) {
        unused(response);

        LOG_DEBUG("ENTER MepServiceSubscriber::DoServiceNotify()");
        LOG_DEBUG("Body Info: %s", request.body().c_str());
        
        do {
        string b = request.body();
        if (b.empty()) {
            break;
        }

        Document d;
        d.Parse(b.c_str());
        if (d.HasParseError()) {
            LOG_ERROR("Parse error: { %d : %lu } %s \n", 
            d.GetParseError(), 
            d.GetErrorOffset(),  
            (char*)rapidjson::GetParseErrorFunc(d.GetParseError()));
            return;
        }          
        list<string> h;
        Value* v;
        
        h = {"serviceReferences", "serName"};
        string serviceName;
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            serviceName = string(v->GetString());
        }

        h = {"serviceReferences", "serInstanceId"};
        string serviceId;
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            serviceId = string(v->GetString());
        }

        h = {"serviceReferences", "state"};
        string status;
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            status = string(v->GetString());
        }
        
        auto it = _servicesInfo.find(serviceId);
        if (it != _servicesInfo.end()) {
            LOG_DEBUG("Change server[ % ]'s status to [ %s ]",serviceName.c_str(),  status.c_str());
            it->second->status = serviceStatusCvs(status);            
        }

        mediator_->feedServicesSubscriberInfo(&_servicesInfo);
        //dynamic_pointer_cast<MepMediator>(_mediator)->feedServicesSubscriberInfo(_servicesInfo);
        } while(0);

        response.send(Http::Code::Ok);
        return;

    }

    void MepServiceSubscriber::rspProcess(const web::http::status_code statusCode, web::http::http_headers& headers, const string& rspBody) {

        LOG_DEBUG("Status code: %u", statusCode);
        for (auto it = headers.begin(); it != headers.end(); ++it) {
            LOG_DEBUG("Headers: %s", it->second.c_str());
        }

        LOG_DEBUG("Response Json Body: %s", rspBody.c_str());

        if (statusCode != web::http::status_codes::Created) {
            //解析body中的失败信息描述；
            ErrorRspBody errBody = parseRspErrorBody(rspBody);
            LOG_ERROR("Response statusCode[ %u ] with detail info [ %s ]", static_cast<unsigned>(statusCode), errBody.detail.c_str());
            return;
        }

        /* 
        if (!rspHeaders.has("Location")) {return;}

        //Location头域携带了订阅ID信息；
        shared_ptr<Http::Header::Header> location = rspHeaders.get("Location");
        ostringstream oss;
        location->write(oss);
        cout<<"Location: "<<oss.str()<<std::endl;
        */

        //响应body处理，获取订阅的
        //TODO
        Document d;
        d.Parse(rspBody.c_str());
        if (d.HasParseError()) {
            LOG_ERROR("Json Parse error: { %d : %lu } %s",
                    d.GetParseError(), 
                    d.GetErrorOffset(), 
                    (char *)rapidjson::GetParseErrorFunc(d.GetParseError()));
            return;
        }
            
        auto cbrItr = d.FindMember("callbackReference");
        string cbr = string(cbrItr->value.GetString());
        
        auto fcItr = d.FindMember("filteringCriteria");
        if (fcItr != d.MemberEnd()) {
            string sn;
            if (fcItr->value.HasMember("serNames")) {
                auto& services = fcItr->value.FindMember("serNames")->value;
                sn = string(services[0].GetString());
            }

            string sid;
            if (fcItr->value.HasMember("serInstanceIds")) {
                auto& servicesId = fcItr->value.FindMember("serInstanceIds")->value;
                sid = string(servicesId[0].GetString());
            } else {
                sid = "";
                LOG_DEBUG("serInstanceId of service Name [ %s ]is not existed, it means that the service is not register on API-GW.", sn.c_str());
            }

            if (!sid.empty()) {
                _servicesInfo.insert(make_pair(sid , make_shared<ServiceSubscriberInfo>(sn, cbr, ServiceStatus::PENDING)));

                auto it = _servicesInfo.find(sid);
                if (it != _servicesInfo.end()) {
                    LOG_DEBUG("Service subscriber result: Name[ %s ], CallbackRef[ %s ]", 
                             it->second->serviceName.c_str(), it->second->callbackRef.c_str());
                }
            }

           ++_commpleted;
        }
    }

    void MepServiceSubscriber::abnormity() {
        result_ = {MepProduceType::DOALL, CRANE_FAIL};
        //result_ = CRANE_FAIL;
        mediator_->MepServiceResult(shared_from_this());
    }
}
