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
