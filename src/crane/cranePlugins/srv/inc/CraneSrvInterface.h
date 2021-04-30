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

#ifndef __PLUGIN_CRANESRVINTERFACE_H__
#define __PLUGIN_CRANESRVINTERFACE_H__

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "PluginBaseInterface.h"

using namespace std;
using namespace Pistache;

namespace NS_CRANE {
//template <typename Result, typename... Args>
class Itf_CraneSrv : public PluginBase {
    public:
        enum class ThreadMode{
            MAIN_THREAD = 0,
            NEW_THREAD,
        };

        virtual unsigned create(unsigned short port) = 0;

        virtual Http::Endpoint::Options& options() = 0;

        virtual unsigned initialize() = 0;

        virtual unsigned router(HttpMethod method, const string& path, Rest::Route::Handler) = 0;

        virtual unsigned handler() = 0;

        virtual unsigned handler(const shared_ptr<Http::Handler> handler) = 0;

        virtual unsigned serverTLS(const string& serverCert, const string& serverKey) = 0;

        virtual unsigned clientTLS(const string& CACert) = 0;
        
        virtual void start(ThreadMode mode) = 0;
        
        virtual unsigned shutdown() = 0;
        
        virtual shared_ptr<Http::Endpoint> endpoint() = 0;

};

}

#endif
