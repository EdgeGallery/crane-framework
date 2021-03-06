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

#ifndef  __CRANE_MEPSERVERPROCESS_H__
#define  __CRANE_MEPSERVERPROCESS_H__

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Util.h"
#include "Log.h"
#include "AbsMepService.h"

using namespace std;
using namespace Pistache;
using namespace rapidjson;

namespace NS_CRANE {

    //这个类名应该改为服务订阅通知
    class MepServerProcess : public AbstractMepService, public enable_shared_from_this<MepServerProcess> {
//            using processFunc = void (*)(Args...);
        public:
            MepServerProcess(shared_ptr<AbstractMediator> mediator, std::string appInstanceId) : AbstractMepService(mediator, appInstanceId) {
                
                //string::size_type pos = _hostUrl.find_last_of(':');
                //string port(_hostUrl.substr(++pos));
                //Address addr(Ipv4::any(), Port(port));
                Address addr(Ipv4::any(), Port(portStr_()));
                LOG_INFO("Server interact with API-GW with IP: [ %s ] and Port: [ %d ]", addr.host().c_str(), addr.port());

                _httpEndpoint = make_shared<Http::Endpoint>(addr);
            }

            MepClazzType ClazzType() override;

            void doInteraction(MepProduceType type) override;

            void Init();
            void Start();

            template <typename Result, typename Cls, typename... Args, typename Obj>
            void SetRoutes(HttpMethod method, const string& path, Result (Cls::*func)(Args...), Obj obj) {
                switch (method) {
                    case HttpMethod::POST:
                        Rest::Routes::Post(_router, path, Rest::Routes::bind(func, obj));
                        break;

                    case HttpMethod::PUT:
                        Rest::Routes::Put(_router, path, Rest::Routes::bind(func, obj));
                        break;
                    
                    case HttpMethod::GET:
                        ;
                        break;
                    
                    case HttpMethod::DELETE:
                        ;
                        break;
                    
                    default:
                        break;
                }
                return;
            }

            template <typename Result, typename Cls, typename... Args, typename Obj>
            void SetRoutes(HttpMethod method, const string& path, Result (Cls::*func)(Args...), shared_ptr<Obj> objPtr) {
                switch (method) {
                    case HttpMethod::POST:
                        Rest::Routes::Post(_router, path, Rest::Routes::bind(func, objPtr));
                        break;

                    case HttpMethod::PUT:
                        Rest::Routes::Put(_router, path, Rest::Routes::bind(func, objPtr));
                        break;
                    
                    case HttpMethod::GET:
                        ;
                        break;
                    
                    case HttpMethod::DELETE:
                        ;
                        break;
                    
                    default:
                        break;
                }
                return;
            }
            
        private:
            shared_ptr<Http::Endpoint> _httpEndpoint;
            
            Rest::Router    _router;

    };
}

#endif
