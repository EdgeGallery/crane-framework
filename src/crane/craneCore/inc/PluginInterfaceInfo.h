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

#ifndef __CRANE_PLUGININTERFACEINFO_H__
#define __CRANE_PLUGININTERFACEINFO_H__

#include <string>
#include <list>
#include <string.h>
#include <memory>

#include "craneMacro.h"
#include "Util.h"
#include "AbsInterfaceInfo.h"
#include "AbsPluginFactory.h"

using namespace std;

namespace NS_CRANE {
class PluginInterfaceInfo {
    public:
        using PluginFactoryList = list<shared_ptr<AbstractPluginFactory>>;

        using PluginLibFileMap = map<string/*pluginName(class name of the plugin)*/, string /*absolute library filename*/>;

        PluginInterfaceInfo(const string& type, const string& curVer, const string& reqFwVer) 
                            : _type(type), _curVer(curVer), _reqFwVer(reqFwVer) {}

        /**
         * @Descripttion: Get plugin interface name which is the key of plugin interface in the registry.
         * @Param: null
         * @Return: plugin interface name.
         */        
        inline const string& type() const {
            return _type;
        }

        /**
         * @Descripttion: Get the version of the interface of the plugin.
         * @Param: null
         * @Return: the version of tht interface.
         */        
        inline const string& curVersion() const {
            return _curVer;
        } 

        /**
         * @Descripttion: Get the required the version of the Crane plugin framework for this plugin interface.
         * @Param: null
         * @Return: Required the version of Crane plugin framework.
         */        
        inline const string& requireFwVer() const {
            return _reqFwVer;
        }

        unsigned validateVersion(const string interfaceType) = delete;

        inline const PluginLibFileMap& pluginLibFileMap() const {
            return _pluginLibFileMap;
        }

        inline const PluginFactoryList& pluginFactoryList() const {
            return _pluginFactoryList;
        }

        /**
         * @Descripttion: Insert a new pair of plugin name and absolute file name into the map
         * @Param: pluginName: plugin class name
         * @Param: filenname: absolute library file name of plugin.
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        unsigned addPluginLibFileMap(const string& pluginName, const string& filename);

        /**
         * @Descripttion: Merge maps together.
         * @Param: : pluginLibFileMap: plugin library file map which will be merged together.  
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        unsigned addPluginLibFileMap(const PluginLibFileMap& pluginLibFileMap);

        string findPluginLibFileMap(const string& pluginName);

        unsigned delPluginLibFileMap(const string& pluginName);

        unsigned addPluginFactory(shared_ptr<AbstractPluginFactory> pPluginFactory);

        unsigned addPluginFactory(const PluginFactoryList&);

        unsigned delPluginFactory(const string& pluginName);

        shared_ptr<AbstractPluginFactory> pluginFactory(const string& pluginName);

    private:
        string              _type;              //plugin interface name

        string              _curVer;            //plugin interface version

        string              _reqFwVer;          //plugin framework version required by this interface.

        PluginFactoryList   _pluginFactoryList; //Contained the plugin factory instance of plugin interface implement.

        PluginLibFileMap    _pluginLibFileMap;  //Contained the plugin name(class name of the plugin implemention) and absolute filename of library. 
};

}

#endif