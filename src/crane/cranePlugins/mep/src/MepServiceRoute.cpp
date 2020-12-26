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

#include "MepServiceRoute.h"

namespace NS_CRANE {
    const std::string MepServiceRoute::_SERVICE_ROUTE_BODY = "{\
	\"appId\": \"TC_APPF_APIGW_PUT_Routes_00\", \
	\"serInfo\": {\
		\"serName\": \"test\",\
		\"addresses\":[{\"host\": \"19.1.56.14\", \"port\": 8080} ]\
	},\
	\"netType\": \"5GCORE_SERVICE\",\
	\"interfaces\": [{\
		\"protocol\": \"http\", \
		\"path\": \"/api/v1\", \
		\"methods\": [\"GET\", \"POST\"],\
		\"description\": \"image retrieval service\",\
		\"pfmMetrics\": \"tt\"\
	    }]\
    }";

    const string MepServiceRoute::_SERVICE_PATH = "/mep/appMng/v1/routes/";
    const string MepServiceRoute::_ROUTE_ID = "img-retrieval-servicer-routeid-1";
    
    MepClazzType MepServiceRoute::ClazzType() {
        return MepClazzType::ServiceRoute;
    }

    void MepServiceRoute::doInteraction(MepProduceType type) {
        LOG_DEBUG("ENTER MepServiceRoute::doInteraction");

        result_ = {type, CRANE_FAIL};
        //result_ = CRANE_FAIL;

        _commpleted = 0;

        for (auto s : _services) {
            Document d;
            d.Parse(_SERVICE_ROUTE_BODY.c_str());
            if (d.HasParseError()) {
                LOG_ERROR("Parse error: { %d : %lu } %s \n",
                d.GetParseError(),
                d.GetErrorOffset(),
                (char*)rapidjson::GetParseErrorFunc(d.GetParseError()));
                return;
            }
                       
            auto& a = d.GetAllocator();

            Value* v;
            list<string> h;
            
            //set appInstanceId
            h = {"appId"};
            v = Util::fatchJsonValue(d, h);
            if (v != nullptr) {
                v->SetString(appInstanceId_.c_str(), a);
            }

            //set serName.
            h = {"serInfo", "serName"};
            v = Util::fatchJsonValue(d, h);
            if (v != nullptr) {
                v->SetString(s.c_str(), a);
            }

            //set addresses
            h = {"serInfo", "addresses"};
            v = Util::cleanJsonArray(d, h);
            if ((v != nullptr) && (v->IsArray())) {
                //string ip("2.2.2.2"); unsigned port = 8080;
                Value addr(rapidjson::kObjectType);
                Value ipv(host_().c_str(), static_cast<SizeType>(host_().size()), a);
                Value portv(portUint_());
                //Value portv(port.c_str(), static_cast<SizeType>(port.size()), a);
                addr.AddMember("host", ipv, a); 
                addr.AddMember("port", portv, a);
                v->PushBack(addr, a);
            }

            Util::printJson(d);

            client_ = make_shared<http_client>(U(HTTPS + endpoint_), clientConfig_);
            LOG_DEBUG("Connect to %s", (HTTPS + endpoint_).c_str());

            //string routeId = "img-retrieval-servicer-routeid-1";
            uri_builder builder = uri_builder();
            builder.append(U(_SERVICE_PATH + _ROUTE_ID));
            LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

            //http_request req(methods::POST);
            //req.set_body(Util::Json2String(d));
            auto rsp = client_->request(methods::PUT, builder.to_string(), Util::Json2String(d)).get();

            const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();

            rspProcess(rsp.status_code(), rspBody);

        }

        if (_commpleted == _services.size()) {
            result_ = {MepProduceType::DOALL, CRANE_SUCC};
            //result_ = CRANE_SUCC;
        } else {
            result_ = {MepProduceType::DOALL, CRANE_FAIL};
            //result_ = CRANE_FAIL;
        }
        mediator_->MepServiceResult(shared_from_this());
        return;
            //rspProcess(rspBody, rsp.status_code());
/*
            string rspBody;
            Http::Code statusCode;
            Http::Header::Collection rspHeaders;

            auto rsp = _mediator->client.post(postServiceUrl).body(Util::Json2String(d)).send();
            rsp.then(
                [&rspHeaders, &rspBody, &statusCode, &completedRequests, this](Pistache::Http::Response response) {
                    ++completedRequests;
                    fprintf(stdout, "Response code[ %u ]\n", static_cast<unsigned>(response.code()));
                    rspHeaders = response.headers();
                    rspBody = response.body();
                    statusCode = response.code();
                    if (!rspBody.empty()) {
                        fprintf(stdout, "Response body[ %s ]\n", rspBody.c_str());
                        rspProcess(statusCode, rspHeaders, rspBody);
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
    
    void  MepServiceRoute::rspProcess(const web::http::status_code statusCode, const string& rspBody) {
        LOG_DEBUG("Status code: %u", statusCode);

        list<string> h;

        if (statusCode != web::http::status_codes::OK) {
            //解析body中的失败信息描述；
            ErrorRspBody errBody = parseRspErrorBody(rspBody);
            LOG_ERROR("Response statusCode[ %u ] with detail info [ %s ]", static_cast<unsigned>(statusCode), errBody.detail.c_str());
            return;
        }

        ++_commpleted;

        return;
    }

}