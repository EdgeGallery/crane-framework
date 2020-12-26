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

#include "CraneSrv.h"

namespace NS_CRANE {
    const string CraneSrv::_CONFIG_FILENAME = ".crane_plugin_srv.cnf";
    const string CraneSrv::_SERVER_THREAD_NAME = "crane_app_http_server";
    
    unsigned CraneSrv::init() {
        unsigned ret = _parseConfigFile();
        if (ret != CRANE_SUCC) {
            LOG_ERROR("Parse HTTP server plugin config file failed.");
            return ret;
        }

        Address addr(Ipv4::any(), _serverPort);
        _endpoint = make_shared<Http::Endpoint>(addr);

        auto opts = Http::Endpoint::options();
        
        //options from configfile.
        opts.threads(static_cast<int>(_serverThreadNum))
            .maxRequestSize(_maxRequestSize)
            .maxResponseSize(_maxResponseSize)
            .threadsName(_SERVER_THREAD_NAME)
            .flags(Tcp::Options::ReuseAddr);
        
        opts.flags(Tcp::Options::ReuseAddr);
        _endpoint->init(opts);
        
        //Server tls
        if (_serverTls) {
            LOG_DEBUG("Config TLS with cert{ %s }, key{ %s }, compression{ %u }", 
                        _certFile.c_str(), _privateKey.c_str(), _compression);
            _endpoint->useSSL(_certFile, _privateKey, _compression);
        }
        //Client tls
        if (_clientAuth) {
            LOG_DEBUG("Config TLS with CA cert{ %s }", _caFile.c_str());
            _endpoint->useSSLAuth(_caFile);
        }
        return CRANE_SUCC;
    }
    
    unsigned CraneSrv::create(unsigned short port) {
        Address addr(Ipv4::any(), port);
        _endpoint = make_shared<Http::Endpoint>(addr);
        return CRANE_SUCC;
    }

    Http::Endpoint::Options& CraneSrv::options() {
        return _opts;
    }

    unsigned CraneSrv::initialize() {
        _endpoint->init(_opts);
        return CRANE_SUCC;
    }
   
    unsigned CraneSrv::router(HttpMethod method, const string& path, Rest::Route::Handler func) {
        switch (method) {
            case HttpMethod::POST:
                Rest::Routes::Post(_router, path, func);
                break;

            case HttpMethod::PUT:
                Rest::Routes::Put(_router, path, func);
                break;
                
            case HttpMethod::GET:
                Rest::Routes::Get(_router, path, func);
                break;

            case HttpMethod::DELETE:
                Rest::Routes::Delete(_router, path, func);
                break;

            default:
                LOG_ERROR("Invalid http method[ %u ]\n", static_cast<unsigned>(method));
                return CRANE_FAIL;
        }
        return CRANE_SUCC;
    }

    unsigned CraneSrv::handler(const shared_ptr<Http::Handler> handler) {
        _endpoint->setHandler(handler);
        return CRANE_SUCC;
    }

    unsigned CraneSrv::handler() {
        if (!_once) {
            _endpoint->setHandler(_router.handler());
            _once = true;
        }
        return CRANE_SUCC;
    }

    unsigned CraneSrv::serverTLS(const string& serverCert, const string& serverKey) {
        _endpoint->useSSL(serverCert, serverKey);
        return CRANE_SUCC;
    } 

    unsigned CraneSrv::clientTLS(const string& CACert) {
        _endpoint->useSSLAuth(CACert);
        return CRANE_SUCC;
    } 

    void CraneSrv::start(ThreadMode mode) {
        if (mode == ThreadMode::MAIN_THREAD) 
            _endpoint->serve(); 
        else 
            _endpoint->serveThreaded();
    }

    unsigned CraneSrv::shutdown() {
        _endpoint->shutdown();
        return CRANE_SUCC;
    }

    unsigned CraneSrv::_parseConfigFile() {
        const char* homepath = getenv("HOME");
        if (homepath == NULL) { 
            LOG_ERROR("Cannot find the HOME env");
            return CRANE_FAIL; 
        }
        string file = string(homepath) + "/" + _CONFIG_FILENAME;

        Document d;
        if (Util::parseJsonFile(file, d) != CRANE_SUCC) {
            LOG_ERROR("Parse srv config file { %s } failed.", file.c_str());
            return CRANE_FAIL;
        }

        Value* v = nullptr;
        list<string> h;

        //fatch app server config info.
        h = {"server", "port"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _serverPort = static_cast<short unsigned>(v->GetUint());
        }

        h = {"server", "threadNum"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _serverThreadNum = v->GetUint();
        }

        h = {"server", "maxRequestSize"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _maxRequestSize =  v->GetUint();
        }

        h = {"server", "maxResponseSize"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _maxResponseSize = v->GetUint();
        }

        h = {"tls", "serverTls"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _serverTls = v->GetBool();
        }

        h = {"tls", "certFile"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _certFile = string(v->GetString());
        }

        h = {"tls", "privateKey"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _privateKey = string(v->GetString());
        }

        h = {"tls", "clientAuth"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _clientAuth = v->GetBool();
        }

        h = {"tls", "caFile"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _caFile = string(v->GetString());
        }

        h = {"tls", "compression"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            _compression = v->GetBool();
        }
        
        return CRANE_SUCC;
    }

    //CRANE_PLUGIN_DEFINE_SINGLETON("1.0.0",    //插件接口版本
    CRANE_PLUGIN_DEFINE("1.0.0",    //插件接口版本
                    Itf_CraneSrv,
                    "1.0.0",    //要求的插件框架版本   
                    CraneSrv,
                    "crane system plugin, which is a HTTP Server.",
                    "dongyin@huawei.com",
                    "LGPL")    
}