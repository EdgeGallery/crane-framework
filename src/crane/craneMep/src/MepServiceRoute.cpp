#include "MepServiceRoute.h"

namespace NS_CRANE {
    const std::string MepServiceRoute::serviceRouteBody = "{\
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
    
    MepClazzType MepServiceRoute::ClazzType() {
        return MepClazzType::ServiceRoute;
    }

    void MepServiceRoute::doInteraction() {
        LOG_DEBUG("ENTER MepServiceRoute::doInteraction");

        _result = CRANE_FAIL;

        _commpleted = 0;

        for (auto s : _services) {
            Document d;
            d.Parse(serviceRouteBody.c_str());
            auto& a = d.GetAllocator();

            Value* v;
            list<string> h;
            
            //set appInstanceId
            h = {"appId"};
            v = Util::fatchJsonValue(d, h);
            if (v != nullptr) {
                v->SetString(_appInstanceId.c_str(), a);
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
                Value ipv(host().c_str(), static_cast<SizeType>(host().size()), a);
                Value portv(portUint());
                //Value portv(port.c_str(), static_cast<SizeType>(port.size()), a);
                addr.AddMember("host", ipv, a); 
                addr.AddMember("port", portv, a);
                v->PushBack(addr, a);
            }

            Util::printJson(d);

            client_ = make_shared<http_client>(U(HTTPS + _endpoint), clientConfig_);
            LOG_DEBUG("Connect to %s", (HTTPS + _endpoint).c_str());

            string routeId = "img-retrieval-servicer-routeid-1";
            uri_builder builder = uri_builder();
            builder.append(U("/mep/appMng/v1/routes/" + routeId));
            LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

            //http_request req(methods::POST);
            //req.set_body(Util::Json2String(d));
            auto rsp = client_->request(methods::PUT, builder.to_string(), Util::Json2String(d)).get();

            const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();

            rspProcess(rsp.status_code(), rspBody);

        }

        if (_commpleted == _services.size()) {
            _result = CRANE_SUCC;
        } else {
            _result = CRANE_FAIL;
        }
        _mediator->MepServiceResult(shared_from_this());
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