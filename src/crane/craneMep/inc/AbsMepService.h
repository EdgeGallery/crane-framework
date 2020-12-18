/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-01 20:03:37
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 11:25:34
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

    enum class ServiceStatus {
        INVALID = 0,    //代码处理的异常返回状态
        ACTIVE,
        INACTIVE,
        UNKNOW,     //尚未发起订阅；
        PENDING,    //发起了订阅，但为收到订阅通知信息；
    };


    struct ErrorRspBody {
        string      type;
        string      title;
        unsigned    status;
        string      detail;
        string      instance;
        
    };

    class AbstractMediator;
    
    class AbstractMepService {
        public:
            AbstractMepService(shared_ptr<AbstractMediator> mediator, std::string appInstanceId) : _mediator(mediator), _appInstanceId(appInstanceId) {

                _initialize_map();

                char* ep = getenv("MEP_APIGW_ENDPOINT");//MEP API-GW's IP+Port
                char* url = getenv("MEP_APIGW_NOTIFY_URL");//
                if (ep == NULL) {
                    _endpoint = "";
                    LOG_ERROR("Unable get MEP_APIGW_ENDPOINT env.");
                    //TODO: 抛出异常，以便MepMediator在创建该对象的时候能够获取到；
                } else {
                    _endpoint = string(ep);
                } 
                
                if (url == NULL) {
                    _hostUrl = "";
                    LOG_ERROR("Unable get MEP_APIGW_NOTIFY_URL env.");
                    //TODO: 抛出异常，以便MepMediator在创建该对象的时候能够获取到；
                } else {
                    _hostUrl = string(url);
                }


                clientConfig_.set_validate_certificates(false);
            }
            
            virtual ~AbstractMepService() { std::cout<<"~AbstractMepService()"<<std::endl;}

            /**
             * @Descripttion: 提供默认行为，子类提供具体的实现，完成和MEP的交互，然后通知和MEP过程交互的结果
             * @Param: 
             * @Return: 
             */            
            virtual void doInteraction() = 0;
            

            virtual MepClazzType ClazzType() = 0;

            virtual unsigned result() {return _result;}

            ErrorRspBody parseRspErrorBody(const string& body);

        protected:
            const string host() {
                string::size_type pos = _hostUrl.find_first_of(':');
                return _hostUrl.substr(0, pos);
            }

            const string portStr() {
                string::size_type pos = _hostUrl.find_last_of(':');
                return _hostUrl.substr(++pos);
            }

            unsigned portUint() {
                string::size_type pos = _hostUrl.find_last_of(':');
                string port(_hostUrl.substr(++pos));
                return static_cast<unsigned>(atoi(port.c_str()));
            }

        protected:
            shared_ptr<AbstractMediator>    _mediator;

            string                          _appInstanceId;
            unsigned                        _result = CRANE_FAIL;
            string                          _endpoint;          //MEP API-GW's endpoint
            string                          _hostUrl;           //APP's IP+Port which is used to receive notify of service status.

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