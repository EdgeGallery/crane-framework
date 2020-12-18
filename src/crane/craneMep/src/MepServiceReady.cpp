/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-03 15:21:48
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-07-16 12:28:00
 */ 

#include "MepServiceReady.h"
/*
using namespace web;
using namespace web::http;
using namespace web::http::client;
*/
namespace NS_CRANE {
    const std::string MepServiceReady::serviceReadyBody = "{\
      \"indication\": \"READY\"\
    }";
    
    MepClazzType MepServiceReady::ClazzType() {
        return MepClazzType::ServiceReady;
    }
    
    void MepServiceReady::doInteraction() {
        
        _result = CRANE_FAIL;

        //完成和MEP APIGW的服务发现交互过程
        //std::string getServiesUrl = "https://" + _endpoint  + "/applications/" + _appInstanceId + "/confirm_ready";
        //LOG_DEBUG("Service Ready ulr: { %s }", getServiesUrl.c_str());

        try {
            client_ = make_shared<http_client>(U(HTTPS + _endpoint), clientConfig_);
            LOG_DEBUG("Connect to %s", (HTTPS + _endpoint).c_str());

            uri_builder builder = uri_builder();
            builder.append(U("/mec_application_mp1/v1/applications/" + _appInstanceId + "/confirm_ready"));
            LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

            //http_request req(methods::POST, builder.to_string());
            //req.set_body(serviceReadyBody);

            auto rsp = client_->request(methods::POST, builder.to_string(), serviceReadyBody).get();

            const web::http::status_code statusCode = rsp.status_code();
            const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();
            rspProcess(rspBody, statusCode);

            _mediator->MepServiceResult(shared_from_this());
        } catch (exception &e) {
            LOG_ERROR("APP ready process exception: %s", e.what());
        }
        return;
  
/*
        vector<Async::Promise<Http::Response>> responses;
        atomic<size_t> completedRequests(0);
        atomic<size_t> failedRequests(0);

        string rspBody;
        Http::Code statusCode;
        
        auto rsp = _mediator->client.post(getServiesUrl).body(serviceReadyBody).send();
        LOG_DEBUG("Send service ready request with body { %s }.", serviceReadyBody.c_str());
        rsp.then(
            [&rspBody, &statusCode, &completedRequests, this](Pistache::Http::Response response) {
                ++completedRequests;
                LOG_DEBUG("Service ready response status code[ %u ]\n", static_cast<unsigned>(response.code()));
                rspBody = response.body();
                statusCode = response.code();
                if (!rspBody.empty()) {
                    LOG_DEBUG("Service ready response's body { %s }", rspBody.c_str());
                    rspProcess(rspBody, statusCode);
                }
                
            },
            [&](std::exception_ptr exc) {
                LOG_DEBUG("Receive exception when send service ready request.");
                PrintException excPrinter;
                excPrinter(exc);
            }
        );
        LOG_DEBUG("here-----------1");
        responses.push_back(move(rsp));
        
        LOG_DEBUG("here-----------2");
        auto sync = Async::whenAll(responses.begin(), responses.end());
        Async::Barrier<vector<Http::Response>> barrier(sync);
        barrier.wait_for(std::chrono::seconds(5));
        
        LOG_DEBUG("here-----------3");
        if (completedRequests.load() == 1) {
            //If all request has received response, whether it's success or failure, the execution at this stage is considered successfull.
            //MepMediator will process the result and decide what to do next.
            LOG_DEBUG("Reveive service ready response");
            _result = CRANE_SUCC;
        } else {
            _result = CRANE_FAIL;
        }
        _mediator->MepServiceResult(shared_from_this());

        return;
*/        
    }

    /*
    unsigned MepServiceReady::result(void) {
        return _result;
    }
    */
    void MepServiceReady::rspProcess(const string& rspBody, const web::http::status_code statusCode) {
        _result = CRANE_FAIL;

        LOG_DEBUG("Status code: %u", statusCode);
        LOG_DEBUG("Response Json Body: %s", rspBody.c_str());

        do {            
            if ((statusCode != web::http::status_codes::NoContent) && (statusCode != web::http::status_codes::OK)) {
                ErrorRspBody errBody = parseRspErrorBody(rspBody);
                LOG_ERROR("Response statusCode[ %u ] with detail info [ %s ]", statusCode, errBody.detail.c_str());
                _result = CRANE_FAIL;
                break;
            }
            LOG_INFO("Receive service ready response successfully.");
            _result = CRANE_SUCC;
        } while(0);
        return;
    }
    
    void MepServiceReady::abnormity() {
        _result = CRANE_FAIL;
        _mediator->MepServiceResult(shared_from_this());
    }
}