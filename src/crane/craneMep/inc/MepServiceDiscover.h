/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-01 20:14:54
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-07-15 20:27:58
 */ 
#ifndef __CRANE_MEPSERVICEDISCOVER_H__
#define __CRANE_MEPSERVICEDISCOVER_H__


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
    class MepService {
        public:
            MepService(string id, string name) : srvId(id), srvName(name) {}
            string      srvId;
            string      srvName;
            string      version;
            string      state;
            using Endpoints = std::list<string>; //string is include ip and port such as 1.1.1.1:5060
            Endpoints    endpoints;
    };

    using DiscoverServicesResult = std::map<std::string, shared_ptr<MepService>>;

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

            void doInteraction() override;

            const DiscoverServicesResult& getDiscoverResult() {
                return _servicesInfo;
            }

            void rspSingleServiceProcess(const string& rspBody, const web::http::status_code statusCode);

            void rspAllServiceProcess(const string& rspBody, const web::http::status_code statusCode);

            void abnormity();

        private:
            //Pistache::Http::Client  _client;
            vector<string>          _services;

            //Save the information of services fatched from MEP API-GW.
            DiscoverServicesResult  _servicesInfo;

            //Commpleted service discover process number.
            unsigned                _commpleted = 0;
    };
}


#endif