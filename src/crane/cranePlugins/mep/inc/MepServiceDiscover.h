/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-01 20:14:54
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 17:23:50
 */ 
#ifndef __CRANE_MEPSERVICEDISCOVER_H__
#define __CRANE_MEPSERVICEDISCOVER_H__


//#include <pistache/client.h>
//#include <pistache/http.h>
//#include <pistache/net.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Log.h"

#include "CraneMepInterface.h"
#include "AbsMepService.h"

using namespace std;
using namespace rapidjson;

namespace NS_CRANE {

    class MepServiceDiscover : public AbstractMepService, public enable_shared_from_this<MepServiceDiscover>{
        public:
    
            MepServiceDiscover(shared_ptr<AbstractMediator> mediator, 
                                std::string appInstanceId, 
                                const vector<string>& services) : AbstractMepService(mediator, appInstanceId) {
                _services = services;

                //Show the services name which is needed to discovered. it's just for debug.
                for (auto s : _services) {
                    
                    LOG_DEBUG("Need to dsicover the service: [ %s ]", s.c_str());
                } 
            }

            MepClazzType ClazzType() override;

            void doInteraction(MepProduceType type) override;

            const DiscoverServicesResult& getDiscoverResult() {
                return _servicesInfo;
            }

            void rspSingleServiceProcess(const string& rspBody, const web::http::status_code statusCode);

            void rspAllServiceProcess(const string& rspBody, const web::http::status_code statusCode);

            void abnormity();

        private:
            static const string     _SERVICE_PATH;
            static const string     _PARA_NAME; 

            vector<string>          _services;
            DiscoverServicesResult  _servicesInfo;//Save the information of services fatched from MEP API-GW.
            unsigned                _commpleted = 0;//Commpleted service discover process number.
    };
}


#endif