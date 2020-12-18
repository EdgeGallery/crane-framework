/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-14 17:12:53
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-23 10:39:36
 */ 
#include "Config.h"

namespace NS_CRANE {
    void Config::showConfigInfo() {

        LOG_DEBUG("Name[ %s ]", appName.c_str());

        LOG_DEBUG("Log Level[ %s ]", logLevel.c_str());
        LOG_DEBUG("Log Target[ %s ]", logTarget.c_str());

        for (auto regs : regServices) { LOG_DEBUG("regService [ %s ]", regs.c_str()); } 
        for (auto dsvs : dsvServices) { LOG_DEBUG("dsvService [ %s ]", dsvs.c_str()); } 
        for (auto subs : subServices) { LOG_DEBUG("subService [ %s ]", subs.c_str()); } 

        LOG_DEBUG("serverPort [ %u ]", serverPort); 
        LOG_DEBUG("serverThreadNum [ %u ]", serverThreadNum);
        LOG_DEBUG("maxRequestSize [ %u ]", maxRequestSize);
        LOG_DEBUG("maxResponseSize [ %u ]", maxResponseSize);

        LOG_DEBUG("serverTls [ %s ]", serverTls? "true":"false");
        LOG_DEBUG("certFile [ %s ]", certFile.c_str()); 
        LOG_DEBUG("privateKey [ %s ]", privateKey.c_str());

        LOG_DEBUG("clientAuth [ %s ]", clientAuth? "true":"false");
        LOG_DEBUG("caFile [ %s ]", caFile.c_str());
        LOG_DEBUG("ompression[ %s ]", compression? "true":"false");
    }

    void Config::_parseConfigFile() {
        const char* homepath = getenv("HOME");
        if (homepath == NULL) {return;}
        string file = string(homepath) + "/" + CRANE_CONFIG_FILENAME;
        FILE *f = fopen(file.c_str(), "r");
        char buf[1024] = "";
        FileReadStream is(f, buf, sizeof(buf));
        
        Document d;
        d.ParseStream<0, UTF8<>, FileReadStream>(is);
        if (d.HasParseError()) {
            LOG_ERROR("Parse error: { %d : %lu }.", 
            d.GetParseError(), 
            d.GetErrorOffset()/*,  
            (char*)rapidjson::GetParseErrorFunc(d.GetParseError())*/);
            fclose(f);
            return ;
        }

        Value* v = nullptr;

        list<string> h;

        //fetch Demo name.
        h = {"name"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            appName = string(v->GetString());
        }

        //fetch log level.
        h = {"log", "level"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            logLevel = string(v->GetString());
        }

        //fetch log target.
        h = {"log", "target"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            logTarget = string(v->GetString());
        }

        //fatch service discover list.
        h = {"dsvServices"};
        v = Util::fatchJsonValue(d, h);
        if ((v != nullptr) && v->IsArray()) {
            for (auto itr = v->Begin(); itr != v->End(); ++itr) {
                dsvServices.push_back(string(itr->GetString()));
                //_dsvServices.push_back(string(itr->GetString()));
            }
        }


        //fatch service register list.
        h = {"regServices"};
        v = Util::fatchJsonValue(d, h);
        if ((v != nullptr) && v->IsArray()) {
            for (auto itr = v->Begin(); itr != v->End(); ++itr) {
                regServices.push_back(string(itr->GetString()));
                //_regServices.push_back(string(itr->GetString()));
            }
        }

        //fatch service subscriber list.
        h = {"subServices"};
        v = Util::fatchJsonValue(d, h);
        if ((v != nullptr) && v->IsArray()) {
            for (auto itr = v->Begin(); itr != v->End(); ++itr) {
                subServices.push_back(string(itr->GetString()));
                //_subServices.push_back(string(itr->GetString()));
            }
        }

        //fatch app server config info.
        h = {"server", "port"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            serverPort = static_cast<short unsigned>(v->GetUint());
        }

        h = {"server", "threadNum"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            serverThreadNum = v->GetUint();
        }

        h = {"server", "maxRequestSize"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            maxRequestSize =  v->GetUint();
        }

        h = {"server", "maxResponseSize"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            maxResponseSize = v->GetUint();
        }

        h = {"tls", "serverTls"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            serverTls = v->GetBool();
        }

        h = {"tls", "clientAuth"};
        v = Util::fatchJsonValue(d, h);
        if (v != nullptr) {
            serverTls = v->GetBool();
        }

        return;
    }
}