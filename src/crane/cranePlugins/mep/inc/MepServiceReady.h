/*
 *    Copyright 2020 Huawei Technologies Co., Ltd.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef __CRANE_MEPSERVICEREADY_H__
#define __CRANE_MEPSERVICEREADY_H__

#include <exception>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Log.h"
#include "AbsMepService.h"

using namespace std;
using namespace rapidjson;

namespace NS_CRANE {
    class MepServiceReady : public AbstractMepService, public enable_shared_from_this<MepServiceReady>{
        public:
            MepServiceReady(shared_ptr<AbstractMediator> mediator, std::string appInstanceId) : AbstractMepService(mediator, appInstanceId) {}

            MepClazzType ClazzType() override;

            //void doInteraction() override;
            void doInteraction(MepProduceType type) override;

            void rspProcess(const web::http::status_code statusCode, const string& rspBody);

            void abnormity();

        private:
            static const string _SERVICE_READY_BODY;
            static const string _SERVICE_PATH;
            static const string _SERVICE_RESOURCE;

    };
}

#endif