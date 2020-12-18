/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-11 14:25:28
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 17:14:29
 */
#ifndef __CRANE_MEPSERVICEROUTE_H__
#define __CRANE_MEPSERVICEROUTE_H__


#include <pistache/client.h>
#include <pistache/http.h>
#include <pistache/net.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Log.h"
#include "AbsMepService.h"

using namespace std;
using namespace Pistache;
using namespace rapidjson;

namespace NS_CRANE {

    class MepServiceRoute : public AbstractMepService, public enable_shared_from_this<MepServiceRoute>{
        public:
    
            MepServiceRoute(shared_ptr<AbstractMediator> mediator, 
                                std::string appInstanceId, const vector<string>& services) : AbstractMepService(mediator, appInstanceId) {
                _services = services;
            }

            MepClazzType ClazzType() override;

            //void doInteraction() override;
            void doInteraction(MepProduceType type) override;

            void rspProcess(const web::http::status_code statusCode, const string& rspBody);

        private:
            static const string         _SERVICE_ROUTE_BODY;
            static const string         _SERVICE_PATH;
            static const string         _ROUTE_ID;
            
            vector<string>              _services;    //需要注册的服务名称
            unsigned                    _commpleted = 0;//Commpleted service discover process number.
    };
}


#endif