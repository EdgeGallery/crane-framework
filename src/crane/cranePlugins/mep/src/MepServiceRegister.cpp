/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-02 14:48:09
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-12 15:21:36
 */ 
#include "MepServiceRegister.h"

namespace NS_CRANE {
    const std::string MepServiceRegister::_SERVICE_REGISTER_BODY = "{\
    \"serName\": \"###\",    \
    \"serCategory\": {  \
        \"href\": \"/example/catalogue1\",  \
        \"id\": \"id12345\",    \
        \"name\": \"string\",  \
        \"version\": \"2.0\"    \
    },  \
    \"version\": \"1.0\",    \
    \"state\": \"ACTIVE\",    \
    \"transportId\": \"Rest1\",   \
    \"transportInfo\": {    \
        \"id\": \"TransId12345\",   \
        \"name\": \"REST\",     \
        \"description\": \"REST API\",  \
        \"type\": \"REST_HTTP\",    \
        \"protocol\": \"HTTP\",    \
        \"version\": \"2.0\",  \
        \"endpoint\": { \
            \"addresses\" : [   \
                {   \
                    \"host\" : \"###\", \
                    \"port\" : \"###\" \
                }   \
            ]   \
        },   \
        \"security\": { \
            \"oAuth2Info\": {   \
                \"grantTypes\": \"OAUTH2_CLIENT_CREDENTIALS\", \
                \"tokenEndpoint\": \"http://apigw.mep.com/token\" \
            } \
        },  \
        \"implSpecificInfo\": {}    \
    },    \
    \"serializer\": \"JSON\",   \
    \"scopeOfLocality\": \"MEC_SYSTEM\",    \
    \"consumedLocalOnly\": false,   \
    \"isLocal\": true   \
    }";
    
    const string MepServiceRegister::_SERVICE_PATH = "/mep/mec_service_mgmt/v1/applications/";
    const string MepServiceRegister::_SERVICE_RESOURCE = "/services";

    MepClazzType MepServiceRegister::ClazzType() {
        return MepClazzType::ServiceRegister;
    }

    void MepServiceRegister::doInteraction(MepProduceType type) {
        LOG_DEBUG("ENTER MepServiceRegister::doInteraction");

        result_ = {type, CRANE_FAIL};
        //result_ = CRANE_FAIL;

        //std::string postServiceUrl = "http://" + _endpoint  + "/applications/" + _appInstanceId + "/services";

        //vector<Async::Promise<Http::Response>> responses;
        //atomic<size_t> completedRequests(0);
        //atomic<size_t> failedRequests(0);

        _commpleted = 0;

        for (auto s : _services) {
            Document d;
            d.Parse(_SERVICE_REGISTER_BODY.c_str());
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

            //set serName.
            h = {"serName"};
            v = Util::fatchJsonValue(d, h);
            if (v != nullptr) {
                v->SetString(s.c_str(), a);
            }

            //set addresses
            h = {"transportInfo", "endpoint", "addresses"};
            v = Util::cleanJsonArray(d, h);
            if ((v != nullptr) && (v->IsArray())) {
                //string ip("2.2.2.2"); unsigned port = 9090;
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

            uri_builder builder = uri_builder();
            builder.append(U(_SERVICE_PATH + s + _SERVICE_RESOURCE));
            LOG_DEBUG("Rest request resource: %s", builder.to_string().c_str());

            //http_request req(methods::POST);
            //req.set_body(Util::Json2String(d));
            auto rsp = client_->request(methods::POST, builder.to_string(), Util::Json2String(d)).get();

            const string rspBody = rsp.content_ready().get().extract_utf8string(true).get();

            rspProcess(rsp.status_code(), rsp.headers(), rspBody);

        }

        if (_commpleted == _services.size()) {
            result_.retCode = CRANE_SUCC;
            //result_ = {MepProduceType::DOALL, CRANE_SUCC};
            //result_ = CRANE_SUCC;
        } else {
            result_.retCode = CRANE_FAIL;
            //result_ = {MepProduceType::DOALL, CRANE_FAIL};
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
    
    void MepServiceRegister::rspProcess(const web::http::status_code statusCode, web::http::http_headers& headers, const string& rspBody) {

        LOG_DEBUG("Status code: %u", statusCode);
        for (auto it = headers.begin(); it != headers.end(); ++it) {
            LOG_DEBUG("Headers: %s", it->second.c_str());
        }
        LOG_DEBUG("Response Json Body: %s", rspBody.c_str());

        list<string> h;

        if (statusCode != web::http::status_codes::Created) {
            //解析body中的失败信息描述；
            ErrorRspBody errBody = parseRspErrorBody(rspBody);
            LOG_ERROR("Response statusCode[ %u ] with detail info [ %s ]", static_cast<unsigned>(statusCode), errBody.detail.c_str());
            return;
        }

        //处理成功响应
        /*
        if (!rspHeaders.has("Location")) { return; }
        std::shared_ptr<Pistache::Http::Header::Header> location = rspHeaders.get("Location");
        std::ostringstream oss;
        location->write(oss);
        std::cout<<"Location: "<<oss.str()<<std::endl;
        */

        //save service register result;
        Document d;
        d.Parse(rspBody.c_str());
        h = {"serInstanceId"};
        Value* vId = Util::fatchJsonValue(d, h);
        h = {"serName"};
        Value* vName = Util::fatchJsonValue(d, h);

        shared_ptr<RegisterService> regSrvRlt = make_shared<RegisterService>(string(vId->GetString()) , string(vName->GetString()));
        auto loc = headers.find("Location");
        if (loc != headers.end()) {
            LOG_DEBUG("Location: %s", loc->second);
            regSrvRlt->location = loc->second;
        }

        _servicesInfo.insert(make_pair(regSrvRlt->srvName, regSrvRlt));

        auto itr = _servicesInfo.find(regSrvRlt->srvName);
        if (itr != _servicesInfo.end()) {
            LOG_DEBUG("Service register info: ID[ %s ], Name[ %s ], Location[ %s ]", 
                        itr->second->srvId.c_str(), itr->second->srvName.c_str(), itr->second->location.c_str());
        }

        ++_commpleted;

        return;
    }

    void  MepServiceRegister::abnormity() {
        result_ = {MepProduceType::DOALL, CRANE_FAIL};
        //result_ = CRANE_FAIL;
        mediator_->MepServiceResult(shared_from_this());
    }
}