/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-14 17:09:08
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-23 09:54:11
 */ 
#ifndef __CRANECONFIG_H__
#define __CRANECONFIG_H__

#include <cstdio>
#include <string>
#include <iostream>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

#include "Log.h"
#include "Util.h"

using namespace std;
using namespace rapidjson;

namespace NS_CRANE {
    class Config {
        public:
            Config() {
                _parseConfigFile();
            }

            void showConfigInfo();
            
            string          appName;

            string          logLevel;
            string          logTarget;
            
            vector<string>  regServices;
            vector<string>  dsvServices;
            vector<string>  subServices;

            short unsigned        serverPort;
            unsigned        serverThreadNum;
            unsigned        maxRequestSize;
            unsigned        maxResponseSize;

            bool            serverTls;  //whether app server would employ TLS over TCP.
            string          certFile;   //app server cert file path and filename.
            string          privateKey; //app server private key file path and filename.
            bool            clientAuth; //whether app server would authenticate client.
            string          caFile;     //ca file path and filename for client auth.
            bool            compression;//whether app server would compress the message whit client.

        private:
            void _parseConfigFile();

            
    };
}

#endif    