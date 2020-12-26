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

#ifndef CRANE_ABSREGISTRY_H
#define CRANE_ABSREGISTRY_H

//#define __x86_64__

#include <string>
#include <map>
#include "AbsLibrary.h"
#include "AbsPluginFactory.h"
#include "AbsInterfaceInfo.h"
#include "PluginInterfaceInfo.h"

namespace crane {

class AbsRegistry {
    public:
        //////////////////////////////////////////////////
        /// \brief 将插件接口注册到插件系统中
        //////////////////////////////////////////////////
        virtual unsigned int registry(AbstractInterfaceInfo&) = 0;

        //////////////////////////////////////////////////////
        /// \brief 将插件动态库的插件工厂和插件实现加载到插件系统中
        //////////////////////////////////////////////////////
        virtual unsigned int load(AbsLibrary&) = 0;

        //////////////////////////////////////////////////////
        /// \brief 扫描路径下的插件so文件
        //////////////////////////////////////////////////////
        virtual unsigned int scan() = 0;

    protected:
        using PluginInterfaceMap = std::map<std::string, PluginInterfaceInfo>;
        PluginInterfaceMap _pluginItfMap;

        using PluginImplementMap = std::map<std::string,  AbsLibrary>;
        PluginImplementMap pluginMap;
        
};

}
#endif