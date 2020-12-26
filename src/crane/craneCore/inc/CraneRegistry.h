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

#ifndef __CRANE_CRANEREGISTRY_H__
#define __CRANE_CRANEREGISTRY_H__

#include <dirent.h>
#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <memory>
#include <mutex>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>
#include "rapidjson/filewritestream.h"

#include "craneMacro.h"
#include "Log.h"
#include "Util.h"
#include "cranePlugin.h"
#include "AbsRegistry.h"
#include "DlLibrary.h"
#include "PluginInterfaceInfo.h"

using namespace std;
using namespace rapidjson;

namespace NS_CRANE {

class CraneRegistry {
    public:
        CraneRegistry() = default;

        ~CraneRegistry() {
            cout<<"~CraneRegistry()"<<endl;
            //TAKE CARE: itfMap must release before libMap, because itfMap contain the pointer of the plugin factory.
            //When I release(::dlclose) the library first and then release pointer of the plugin factroy, process can 
            //not find memory of plugin factory.
            _pluginItfMap.clear();
            _pluginLibMap.clear();
        }

        /**
         * @Descripttion: Initialize the crane system.
         * @Param: 
         * @Return:  
         */        
        unsigned craneInit(int argc, char** argv);

        /**
         * @Descripttion: Find plugin interface info from _pluginItfMap with plugin interface name.
         * @Param: plugin interface name 
         * @Return: a copy of the shared_ptr<PluginInterfaceInfo>
         */        
        shared_ptr<PluginInterfaceInfo> findPluginItfInfo(const string&);
        
        /**
         * @Descripttion: Create a new Itf object through absolute library file name.
         * @Param: absoluteFileName: Absolute library file name of the plugin.
         * @Return: shared_ptr<PluginInterfaceInfo>
         */        
        shared_ptr<PluginInterfaceInfo> createItfInfo(const string& absoluteFileName);

        /**
         * @Descripttion: Clear the data related with the plugin library file which has been load before.
         *                1. Clear the lib instance from registry's map.
         *                2. Clear the absolute library file name from _pluginLibFileMap of the Itf instance.
         *                3. Clear the plugin factory instance from _pluginFactoryList of the Itf instance.
         * @Param: type: Interface name of the plugin 
         * @Param: pluginName: class name of the plugin 
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        unsigned clearPlugin(const string& type, const string& pluginName);

    protected:
        /**
         * @Descripttion: Add plugin interface info into the registry. 
         * @Param: instance of the shared_ptr<PluginInterfaceInfo>
         * @Return: CRANE_SUCC/CRANE_FAIL 
         */        
        unsigned registry_(shared_ptr<PluginInterfaceInfo>);

    private:
        using PluginInterfaceMap = map<string /*interface name*/, shared_ptr<PluginInterfaceInfo>>;
        PluginInterfaceMap _pluginItfMap; //map contain pair of interface name and interface instance.

        using PluginImplementMap = map<string /*libName*/,  unique_ptr<DlLibrary>>; //libName is ABCD which is extracted from libcranepluginABCD.so
        PluginImplementMap _pluginLibMap; //map contain pair of library file name and library handle.
        
        static const string     _CACHE_FILE;
        static const string     _CACHE_FILE_TMP;

        string                  _crane_sys_plugin_path;
        string                  _crane_app_plugin_path;

        enum class _InitMode {
            LIB_FILE   = 0,
            CACHE_FILE,
        };

        mutex                   _mtx;

        list<string>            _dlFiles; //library files.

        unsigned _addPluginLibMap(unique_ptr<DlLibrary>);

        /**
         * @Descripttion: Remove the library from _pluginLibMap and close the handle of the library.
         * @Param:  filename: Absolate filename of the plugin library
         * @Return: CRANE_SUCC/CRANE_FAIL 
         */        
        unsigned _delPluginLibMap(const string& filename);

        bool _isValidLibName(const string& path);
        
        /**
         * @Descripttion:   扫描插件路径下的插件动态库文件，将剔除lib前缀和.so后缀的文件名写入dlFiles
         * @Param:          list: 动态库文件名列表
         * @Param:          folder: 动态库路径名称
         * @Param:          extension: 动态库后缀名
         * @Param:          recursive: 是否在动态库路径名下进行递归处理
         * @Return:         void 
         */        
        void _listFiles(list<string>& list, const string& folder, const string& extension, bool recursive);

        _InitMode _initMode();

        void _initPluginPath();

        unsigned _scan();

        /**
         * @Descripttion: 
         * @Param: 
         * @Return: 
         */        
        unsigned _loadFromLib();

        unsigned _createCacheFile();

        /**
         * @Descripttion: Just load the information from cach (json) file, which include the information of the interface and library file of the plugin.
         *                Plugin library and plugin factory will load when create the plugin instance at the first time.
         * @Param: null
         * @Return: 
         */        
        unsigned _loadFromCache();

        /**
         * @Descripttion: Parse the cache file and fill the information of the interface and library file of the plugin into the registry.
         * @Param: null
         * @Return: 
         */        
        unsigned _parseCacheFile();
};

}

#endif