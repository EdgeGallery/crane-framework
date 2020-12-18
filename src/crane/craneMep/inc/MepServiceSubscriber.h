/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-08 18:49:01
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 11:39:55
 */ 
#ifndef  __CRANE_MEPSERVICESUBSCRIBER_H__
#define  __CRANE_MEPSERVICESUBSCRIBER_H__

#include <vector>
#include <map>

#include <pistache/router.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Util.h"
#include "AbsMepService.h"
//#include "MepMediator.h"    //TODO:

using namespace std;
using namespace Pistache;
using namespace rapidjson;

namespace NS_CRANE {

    class ServiceSubscriberInfo {
        public:
            ServiceSubscriberInfo(const string& sn, const string& ref, const ServiceStatus st)
                :serviceName(sn), callbackRef(ref), status(st) {}

            string              serviceName;
            string              callbackRef;
            ServiceStatus       status;
    };

    using ServicesSubscriberInfo = map<string, shared_ptr<ServiceSubscriberInfo>>;

    class MepServiceSubscriber : public AbstractMepService, public enable_shared_from_this<MepServiceSubscriber> {
        public:
            static const std::string clazzType;

            MepServiceSubscriber(shared_ptr<AbstractMediator> mediator, std::string appInstanceId, const vector<string>& services) : AbstractMepService(mediator, appInstanceId) {
                _services = vector<string>(services);
                
            }

            MepClazzType ClazzType() override;

            void doInteraction() override;

            ServicesSubscriberInfo& getServicesInfo() {return _servicesInfo;}

            void DoServiceNotify(const Rest::Request& request, Http::ResponseWriter response);

            void rspProcess(const web::http::status_code statusCode, web::http::http_headers& headers, const string& rspBody);

            void abnormity();

            const string getCallbackRef() {
                return "http://" + _hostUrl + "/" + callbackRef;
            }

            const string getCallbackRefResource() {
                return "/" + callbackRef;
            }

        private:
           static const string serviceSubscriberBody;
           static const string callbackRef;
           vector<string> _services;
           ServicesSubscriberInfo _servicesInfo;

           unsigned                     _commpleted = 0;
    };
    
}
#endif