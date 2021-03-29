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
#include "Wrapper.h"

using namespace std;
using namespace rapidjson;

namespace NS_CRANE {

class CraneRegistry {
    public:
        CraneRegistry() {
            // Add add null swappable plugin instance in order to return it when cannot find instance by id.
            _pluginSwappableInstanceMap.insert(
                make_pair(NULL_PLUGIN_SWAPPABLE_ID, 
                        make_pair(make_shared<Wrapper>(), 
                                  make_pair(NULL_PLUGIN_ID, NULL_PLUGIN_SWAPPABLE_ID))));
        }


        ~CraneRegistry() {
            cout<<"~CraneRegistry()"<<endl;

            // Release the wrapped plugin instanse explicitly, before
            // release the _pluginInstanceMap.
            _pluginSwappableInstanceMap.clear();

            // Release the plugin instance explicitly, before 
            // release the _pluginItfMap and _pluginLibMap.
            _pluginInstanceMap.clear();

            // TAKE CARE: itfMap must release before libMap, because itfMap contain the pointer of the plugin factory.
            // When I release(::dlclose) the library first and then release pointer of the plugin factroy, process can 
            // not find memory of plugin factory.
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
        shared_ptr<PluginInterfaceInfo> findPluginItfInfo(const string&) const;

        // Add Swap dongyin 3-5
        /**
         * @Descripttion: Create a new Itf object through absolute library file name.
         * @Param: absoluteFileName: Absolute library file name of the plugin.
         * @Param[out]: desc:  Description of the plugin.
         * @Return: shared_ptr<PluginInterfaceInfo>
         */        
        shared_ptr<PluginInterfaceInfo> createItfInfo(const string& absoluteFileName, PluginDesc& desc);
        
        /**
         * @Descripttion: Create a new Itf object through absolute library file name.
         * @Param: absoluteFileName: Absolute library file name of the plugin.
         * @Return: shared_ptr<PluginInterfaceInfo>
         */        
        shared_ptr<PluginInterfaceInfo> createItfInfo(const string& absoluteFileName);


        /**
         * @Descripttion: Clear the data related with the plugin library file which has been load before.
         *                If the instance id is existed in the PluginInterfaceInfo, it will do nothing.
         *                1. Clear the lib instance from registry's map.
         *                2. Clear the absolute library file name from _pluginLibFileMap of the Itf instance.
         *                3. Clear the plugin factory instance from _pluginFactoryList of the Itf instance.
         * @Param: type: Interface name of the plugin 
         * @Param: pluginName: class name of the plugin 
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        unsigned clearPlugin(const string& type, const string& pluginName);

        /**
         *  Begin: Methods of the plugin instance management, which is related with
         *  PluginInstanceMap _pluginInstanceMap and PluginInterfaceInfo _pluginInstanceIdMap.
         */

        /**
         * @Descripttion: Insert the plugin id and plugin instance, itf type, pluginName
         *              into the _pluginInstanceMap.
         *              At the same time, insert the plugin id into plugin instance
         *              map of the pluginInterfaceInfo.
         * @Param: id: plugin id.
         * @Param: inst: plugin instance.
         * @Param: itfType: interface type name of plugin instance.
         * @Param: pluginName: implemention class name of the plugin.
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        unsigned addPluginInstance(const string& id, 
                                   shared_ptr<PluginBase> inst, 
                                   const string& itfType, 
                                   const string& pluginName);

        /**
         * @Descripttion: Clear the plugin id from plugin instance map and
         *              plugin id map in the PluginInterfaceInfo. 
         * @Param: id: plugin instance id. 
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        unsigned delPluginInstance(const string& id);

        /**
         * @Descripttion: Whether the plugin id is exist in the pluginInstance map. 
         * @Param: id: plugin instance id. 
         * @Return: true/false
         */        
        bool isExistPluginInstance(const string& id);

        /**
         * @Descripttion: Get plugin id through interface type and plugin name.
         *              When there is more than one plugin id, then return the
         *              first one of list.
         * @Param: itfType: interface type. 
         * @Param: pluginName: plugin implement class name.
         * @Return: plugin instance id
         */        
        const string getPluginId(const string& itfType, const string& pluginName) const;        

        /**
         * @Descripttion: Get the pair of interface type and class name of the plugin.
         * @Param: id: plugin instance id. 
         * @Return: std::pair<type, pluginName> 
         */        
        pair<string, string> getPluginItfAndImplName(const string& id);

        /**
         * @Descripttion: Get plugin instance through plugin instance id.
         * @Param: id: plugin instance id. 
         * @Return: plugin instance 
         */        
        shared_ptr<PluginBase> getPluginInstance(const string& id);

        /**
         * @Descripttion: Get plugin instance through type and plugin name.
         * @Param: id: plugin instance id. 
         * @Return: plugin instance 
         */        
        shared_ptr<PluginBase> getPluginInstance(const string& itfType, 
                                                const string& pluginName);

        /**
         * @Descripttion: Release the plugin instance by plugin id.
         *              If the plugin instance is not referd by other object, then 
         *              delete the id from plugin instance id map of the pluginInterfaceInfo and 
         *              release the entry in the plugin instance map.
         * @Param: Instance id of the shared_ptr<PluginInterfaceInfo>
         * @Return: NULL 
         */  
        void relPluginInstance(const string& id);    

        /**
         * @Descripttion: Release all plugin instances 
         * @Param: null
         * @Return: NULL 
         */  
        void relPluginInstance();            

        /**
         *  End: Methods of plugin instance management.
         */


        /**
         *  Begin: Methods of the wrapped plugin instance management, which is 
         *  related with PluginSwappableInstanceMap _pluginSwappableInstanceMap.
         */
        /**
         * @Descripttion: Insert the wrapped plugin instance into the map.
         * @Param: id: Wrapped plugin id.
         * @Param: wsp: Wrapped plugin instance.
         * @Param: id: Wrapped plugin id.
         * @Return: NULL 
         */  
        unsigned addPluginSwappableInstance(const string& id, 
                                            shared_ptr<Wrapper> wsp,
                                            const string& pluginId, 
                                            const string& description);

        /**
         * @Descripttion: Get the wrapped plugin instance from the map. 
         *              If id is not existed in the map, then it return the 
         *              Wrapper object created by default constructor which is 
         *              fected by NULL_PLUGIN_SWAPPABLE_ID.
         * @Param: id: Wrapped plugin id.
         * @Return: the instance of wrapped plugin.
         */  
        shared_ptr<Wrapper> getPluginSwappableInstance(const string& id);

        /**
         * @Descripttion: Release the wrapped plugin instance from the map,
         *              if it is not referred by the others.
         *              It will try to release the plugin instance by plugin id,
         *              if it is not referred by the others.
         * @Param: id: Wrapped plugin id.
         * @Return: NULL
         */  
        void relPluginSwappableInstance(const string& id);
        /**
         *  End: Methods of wrapped plugin instance management.
         */

    protected:
        /**
         * @Descripttion: Add plugin interface info into the registry. 
         * @Param: instance of the shared_ptr<PluginInterfaceInfo>
         * @Return: CRANE_SUCC/CRANE_FAIL 
         */        
        unsigned registry_(shared_ptr<PluginInterfaceInfo>);

    private:
        using PluginInterfaceMap = map<string /*interface name*/, 
                                    shared_ptr<PluginInterfaceInfo>>;
        PluginInterfaceMap _pluginItfMap; //map contain pair of interface name and interface instance.

        using PluginLibMap = map<string /*libName*/,  
                                unique_ptr<DlLibrary>>; //libName is ABCD which is extracted from libcranepluginABCD.so
        PluginLibMap _pluginLibMap; //map contain pair of library file name and library handle.

        using PluginInstanceMap = map<string /*plugin instance id*/, 
                                    pair<shared_ptr<PluginBase>/*plugin instance*/, pair<string/*interface name*/, string/*pluginName(class name of the plugin)*/>>>;
        PluginInstanceMap _pluginInstanceMap; // Contained the plugin instance.

        //using PluginSwappableInstanceMap = map<string /*id*/, pair<shared_ptr<Wrapper>, string>>;
        //PluginSwappableInstanceMap _pluginSwappableInstanceMap;
        using PluginSwappableInstanceMap = map<string /*wrapped plugin id*/, 
                                    pair<shared_ptr<Wrapper>, pair<string/*plugin id*/, string/*description*/>>>;
        PluginSwappableInstanceMap _pluginSwappableInstanceMap;
        
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

        /**
         * @Descripttion: Clear the plugin instance id from the plugin instance id map of 
         *              the pluginInterfaceInfo.
         * @Param: type: Interface name of the plugin.
         * @Param: pluginName: class name of the plugin.
         * @Param: id: plugin instance id.
         * @Return: CRANE_SUCC/CRANE_FAIL 
         */ 
        unsigned _clearPluginIdInItfInfo(const string& type, const string& pluginName, const string& id);

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
