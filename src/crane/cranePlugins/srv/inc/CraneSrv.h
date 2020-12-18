/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-12 09:22:32
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-04 16:43:01
 */
#ifndef __PLUGIN_CRANESRV_H__
#define __PLUGIN_CRANESRV_H__


#include "Util.h"
#include "Log.h"

#include "cranePlugin.h"
#include "CraneSrvInterface.h"

using namespace std;
using namespace Pistache;

#define __start(s) do { (s)->endpoint->serve(); } while(0)

namespace NS_CRANE {
//template <typename Result, typename... Args>
class CraneSrv : public Itf_CraneSrv {
    public:
        CraneSrv() : _opts(Http::Endpoint::options()) { }

        /**
         * @Descripttion: Create endpoint and initialize it through config file.
         * @Param: 
         * @Return: 
         */        
        unsigned init() override;

        //template <typename Result, typename... Args>
        //unsigned router(HttpMethod method, const string& path, Result (*func)(Args...)) override;
        //unsigned router(HttpMethod method, const string& path, 
                        /*void (*func)(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter)) override;*/
                        //Rest::Router::Result (*func)(Args...)) override;

        /**
         * @Descripttion: Create a new endpoint and initialize it by code. This endpoint will replace the one created during the init().
         * @Param: port: port number of http server.
         * @Return: options of endpoint.
         */        
        unsigned create(unsigned short port) override; 

        /**
         * @Descripttion: Get option of endpoint.
         * @Param: null
         * @Return: Options.
         */        
        Http::Endpoint::Options& options() override;

        /**
         * @Descripttion: Set option parameter into server.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAILED
         */        
        unsigned initialize() override;

        unsigned router(HttpMethod method, const string& path, Rest::Route::Handler) override;

        unsigned handler() override;
        
        unsigned handler(const shared_ptr<Http::Handler> handler) override;

        unsigned serverTLS(const string& serverCert, const string& serverKey) override;

        unsigned clientTLS(const string& CACert) override;

        void start(ThreadMode mode) override;

        shared_ptr<Http::Endpoint> endpoint() override {
            return _endpoint;
        }
        
        unsigned shutdown() override;

    private:
        static const string         _CONFIG_FILENAME;
        static const string         _SERVER_THREAD_NAME;
        unsigned                    _parseConfigFile();

        short unsigned              _serverPort;
        unsigned                    _serverThreadNum;
        unsigned                    _maxRequestSize;
        unsigned                    _maxResponseSize;

        bool                        _serverTls = false;  //whether app server would employ TLS over TCP.
        string                      _certFile;   //app server cert file path and filename.
        string                      _privateKey; //app server private key file path and filename.
        bool                        _clientAuth; //whether app server would authenticate client.
        string                      _caFile;     //ca file path and filename for client auth.
        bool                        _compression = false;//whether app server would compress the message whit client.
        Rest::Router                _router;
        shared_ptr<Http::Endpoint>  _endpoint;
        typename Http::Endpoint::Options     _opts;

        bool                        _once = false;

};

}

#endif