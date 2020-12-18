/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-03 15:06:25
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 11:35:32
 */ 

#ifndef __CRANE_MEPSERVICEREADY_H__
#define __CRANE_MEPSERVICEREADY_H__

#include <exception>

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
    class MepServiceReady : public AbstractMepService, public enable_shared_from_this<MepServiceReady>{
        public:

            MepServiceReady(shared_ptr<AbstractMediator> mediator, std::string appInstanceId) : AbstractMepService(mediator, appInstanceId) {}

            MepClazzType ClazzType() override;

            void doInteraction() override;

            void rspProcess(const string& rspBody, const web::http::status_code statusCode);

            void abnormity();

        private:
            static const string serviceReadyBody;

    };
}

#endif