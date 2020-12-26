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

#ifndef __CRANE_ABSMEPSERVICE_H__
#define __CRANE_ABSMEPSERVICE_H__

#include <memory>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Log.h"
#include "Util.h"
#include "AbsMediator.h"

#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/details/basic_types.h>

using namespace std;

using namespace rapidjson;

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace NS_CRANE {

    enum class MepClazzType {
        ServiceReady,
        ServiceDiscover,
        ServiceRegister,
        ServiceRoute,
        ServiceSubscirber,
        ServerProcess
    };
    /*
    enum class ServiceStatus {
        INVALID = 0,    //代码处理的异常返回状态
        ACTIVE,
        INACTIVE,
        UNKNOW,     //尚未发起订阅；
        PENDING,    //发起了订阅，但为收到订阅通知信息；
    };
    */

    struct ErrorRspBody {
        string      type;
        string      title;
        unsigned    status;
        string      detail;
        string      instance;
        
    };

    enum class MepProduceType {
        DOALL = 0,  
        DISCOVER,
        REGISTER,
    };
    struct MepProduceResult {
        MepProduceType  produceType;
        unsigned        retCode;
    };

    class AbstractMediator;
    
    class AbstractMepService {
        public:
            AbstractMepService(shared_ptr<AbstractMediator> mediator, std::string appInstanceId) : mediator_(mediator), appInstanceId_(appInstanceId) {

                _initialize_map();

                char* ep = getenv("MEP_APIGW_ENDPOINT");
                char* url = getenv("MEP_APIGW_NOTIFY_URL");
                if (ep == NULL) {
                    endpoint_ = "";
                    LOG_ERROR("Unable get MEP_APIGW_ENDPOINT env.");
                    //TODO: 抛出异常，以便MepMediator在创建该对象的时候能够获取到；
                } else {
                    endpoint_ = string(ep);
                } 
                
                if (url == NULL) {
                    hostUrl_ = "";
                    LOG_ERROR("Unable get MEP_APIGW_NOTIFY_URL env.");
                    //TODO: 抛出异常，以便MepMediator在创建该对象的时候能够获取到；
                } else {
                    hostUrl_ = string(url);
                }

                //set http client config info.
                clientConfig_.set_validate_certificates(false);
            }
            
            virtual ~AbstractMepService() { std::cout<<"~AbstractMepService()"<<std::endl;}

            /**
             * @Descripttion: 提供默认行为，子类提供具体的实现，完成和MEP的交互，然后通知和MEP过程交互的结果
             * @Param: 
             * @Return: 
             */            
            virtual void doInteraction(MepProduceType type) = 0;
            
            virtual MepClazzType ClazzType() = 0;

            virtual MepProduceResult result() {return result_;}

            ErrorRspBody parseRspErrorBody(const string& body);

        protected:
            const string host_() {
                string::size_type pos = hostUrl_.find_first_of(':');
                return hostUrl_.substr(0, pos);
            }

            const string portStr_() {
                string::size_type pos = hostUrl_.find_last_of(':');
                return hostUrl_.substr(++pos);
            }

            unsigned portUint_() {
                string::size_type pos = hostUrl_.find_last_of(':');
                string port(hostUrl_.substr(++pos));
                return static_cast<unsigned>(atoi(port.c_str()));
            }

        protected:
            //unsigned                        result_ = CRANE_FAIL;
            MepProduceResult                result_ = {MepProduceType::DOALL, CRANE_FAIL};
            shared_ptr<AbstractMediator>    mediator_;

            string                          appInstanceId_;
            string                          endpoint_;          //MEP API-GW endpoint
            string                          hostUrl_;           //APP的IP+Port

            shared_ptr<http_client>         client_;
            http_client_config              clientConfig_;

        private:
            array<pair<string, ServiceStatus>, 5> _serviceStatusEnumMap;

            void _initialize_map()
            {
               _serviceStatusEnumMap[0] = std::make_pair("INVALID", ServiceStatus::INVALID);
               _serviceStatusEnumMap[1] = std::make_pair("ACTIVE", ServiceStatus::ACTIVE);
               _serviceStatusEnumMap[2] = std::make_pair("INACTIVE", ServiceStatus::INACTIVE);
               _serviceStatusEnumMap[3] = std::make_pair("UNKNOW", ServiceStatus::UNKNOW);
               _serviceStatusEnumMap[4] = std::make_pair("PENDING", ServiceStatus::PENDING);
            }

        public:
            ServiceStatus serviceStatusCvs(string const& enum_type)
            {
                for(auto val : _serviceStatusEnumMap)
                {
                    if(val.first == enum_type)
                    {
                        return val.second;
                    }
                }

                LOG_ERROR("Invalid service status: %s", enum_type);
                return _serviceStatusEnumMap[0].second;
            }

            string  serviceStatusCvs(ServiceStatus enum_type)
            {
                for(auto val : _serviceStatusEnumMap)
                {
                    if(val.second == enum_type)
                    {
                        return val.first;
                    }
                }

                LOG_ERROR("Invalid service status: %u", enum_type);
                return _serviceStatusEnumMap[0].first;
            }

    };

}

#endif