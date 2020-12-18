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

            void doInteraction() override;

            void rspProcess(const web::http::status_code statusCode, const string& rspBody);

        private:
            static const std::string serviceRouteBody;
            vector<string>           _services;    //需要注册的服务名称

            //Commpleted service discover process number.
            unsigned                _commpleted = 0;
    };
}


#endif