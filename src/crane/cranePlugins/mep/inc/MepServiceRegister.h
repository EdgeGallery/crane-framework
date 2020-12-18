/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-01 20:17:48
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 16:20:12
 */ 

#ifndef __CRANE_MEPSERVICEREGISTER_H__
#define __CRANE_MEPSERVICEREGISTER_H__

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "AbsMepService.h"

using namespace std;
//using namespace Pistache;
using namespace rapidjson;

namespace NS_CRANE {

    class RegisterService {
        public:
            RegisterService(string id, string name) : srvId(id), srvName(name) {}
            string              srvId;
            string              srvName;
            string              location;
            ServiceStatus       status;
    };

    using RegisterServicesResult = map<string, shared_ptr<RegisterService>>;

    class MepServiceRegister : public AbstractMepService, public enable_shared_from_this<MepServiceRegister> {
        public:

            MepServiceRegister(shared_ptr<AbstractMediator> mediator, std::string appInstanceId, const vector<string>& services) : AbstractMepService(mediator, appInstanceId) {
                _services = services;    
            }

            MepClazzType ClazzType() override;

            //void doInteraction() override;
            void doInteraction(MepProduceType type) override;

            const RegisterServicesResult& getRegisterResult() {
                return _servicesInfo;
            }

            void rspProcess(const web::http::status_code statusCode, web::http::http_headers& headers, const string& rspBody);

            void abnormity();
            
        private:
           static const std::string     _SERVICE_REGISTER_BODY;
           static const string          _SERVICE_PATH;
           static const string          _SERVICE_RESOURCE;

           vector<string>               _services;    //需要注册的服务名称
           
           RegisterServicesResult       _servicesInfo;

           //Commpleted service discover process number.
           unsigned                     _commpleted = 0;
    };

}

#endif