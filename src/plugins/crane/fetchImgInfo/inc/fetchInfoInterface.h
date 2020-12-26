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

#ifndef __PLUGIN_FETCHINFOINTERFACE_H__
#define __PLUGIN_FETCHINFOINTERFACE_H__

#include <array>
#include "PluginBaseInterface.h"

using namespace std;

class Itf_FetchInfo : public NS_CRANE::PluginBaseInterface {
    public:
        typedef struct {
            std::string     picName;
            float           gpsDiff;
            float           gpsCount;
            float           gpsTime;
        } Result;

    public:
        virtual unsigned fetchInfo(std::array<float, 4096>&, float longitude, float latitude ,Result&) = 0;
};

#endif