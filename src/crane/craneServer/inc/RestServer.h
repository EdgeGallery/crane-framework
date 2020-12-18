/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-14 11:43:45
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-07-14 17:44:21
 */ 
#ifndef __CRANE_SERVER_H__
#define __CRANE_SERVER_H__

#include <memory>


#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Config.h"
#include "Data.h"
//#include "AbsPluginFrame.h"


using namespace std;
using namespace Pistache;
using namespace rapidjson;

namespace NS_CRANE {
    #if 0
    namespace server {
        class Options {
            friend class Server;
            public:
                Option() {}
                Options& threads(unsigned val) {_threadNum = val; return *this;}
                Options& keepAlive(bool val) {_keepAlive = val; return *this;}
                Options& maxConntions(unsigned val) {_maxConnectionsPerHost = val; return *this;}
                Options& maxResponseSize(size_t val) {_maxResponseSize = val; return *this;}
                
            private:
                unsigned _threadNum = 1;
                bool    _keepAlive = true;
                unsigned  _maxConnectionsPerHost = 8;
                size_t  _maxResponseSize = numeric_limits<uint32_t>::max();

        };
    }
    #endif

    class RestServer {
        public:
            RestServer() { }

            void setAddr(Address addr);

            void init();

            void start();

            RestServer& threadNum(unsigned val) {_threadNum = val; return *this;}
            RestServer& maxRequestSize(size_t val) {_maxRequestSize = val; return *this;}
            RestServer& maxResponseSize(size_t val) {_maxResponseSize = val; return *this;}
            
            Rest::Router router;

            #ifdef BUILD_IMG
            //#if 0
            //static void postProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
            //#endif

            //static void gstPluginCallback(char* key, unsigned char* result);
            #endif

        private:
            shared_ptr<Http::Endpoint> _endpoint;
            //unsigned    _port;
            //Address     _addr;

            unsigned _threadNum = 10;
            string  _threadName = "Crane_APP_Server";
            size_t  _maxResponseSize = 4096;
            size_t  _maxRequestSize = numeric_limits<uint32_t>::max();
        
    };

    #ifdef BUILD_IMG

    void gstPluginCallback(char* key, unsigned char* result);

    void testFunc();

    #endif
}

#endif