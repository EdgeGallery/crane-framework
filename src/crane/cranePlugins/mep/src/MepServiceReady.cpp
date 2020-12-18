/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-03 15:21:48
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-12 15:22:02
 */ 

#include "MepServiceReady.h"

namespace NS_CRANE {
    const string MepServiceReady::_SERVICE_PATH = "/mec_application_mp1/v1/applications/";
    const string MepServiceReady::_SERVICE_RESOURCE = "/confirm_ready";
    const string MepServiceReady::_SERVICE_READY_BODY = "{\
      \"indication\": \"READY\"\
    }";
    
    MepClazzType MepServiceReady::ClazzType() {
        return MepClazzType::ServiceReady;
    }
    
    void MepServiceReady::doInteraction(MepProduceType type) {
        result_ = {type, CRANE_FAIL};
        //result_ = CRANE_FAIL;

        //Report app ready info to api-gw.
        try {
            client_ = make_shared<http_client>(U(HTTPS + endpoint_), clientConfig_);
            LOG_DEBUG("Connect to %s", (HTTPS + endpoint_).c_str());

            uri_builder builder = uri_builder();
            builder.append(U(_SERVICE_PATH + appInstanceId_ + _SERVICE_RESOURCE));
            LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

            auto rsp = client_->request(methods::POST, builder.to_string(), _SERVICE_READY_BODY).get();

            const web::http::status_code statusCode = rsp.status_code();
            const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();
            rspProcess(statusCode, rspBody);

            mediator_->MepServiceResult(shared_from_this());
        } catch (exception &e) {
            LOG_ERROR("APP ready process exception: %s", e.what());
        }
        return;
    }

    void MepServiceReady::rspProcess(const web::http::status_code statusCode, const string& rspBody) { 
        result_ = {MepProduceType::DOALL, CRANE_FAIL};
        //result_ = CRANE_FAIL;

        LOG_DEBUG("Status code: %u", statusCode);
        LOG_DEBUG("Response Json Body: %s", rspBody.c_str());

        do {            
            if ((statusCode != web::http::status_codes::NoContent) && (statusCode != web::http::status_codes::OK)) {
                ErrorRspBody errBody = parseRspErrorBody(rspBody);
                LOG_ERROR("Response statusCode[ %u ] with detail info [ %s ]", statusCode, errBody.detail.c_str());
                result_.retCode = CRANE_FAIL;
                break;
            }
            LOG_INFO("Receive service ready response successfully.");

            result_.retCode = CRANE_SUCC;
        } while(0);
        return;
    }
    
    void MepServiceReady::abnormity() {
        result_.retCode = CRANE_FAIL;
        mediator_->MepServiceResult(shared_from_this());
    }
}