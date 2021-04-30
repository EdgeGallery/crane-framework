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
    using PluginFactoryList = list<shared_ptr<IPluginFactory>>;

    using PluginLibFileMap = map<string/*pluginName(class name of the plugin)*/, 
                                string /*absolute library filename*/>;

    using PluginInstanceIdMap = map<string/*pluginName(class name of the plugin)*/, 
                                    list<string> /*list of instance id*/>;

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

    /**
     * @Descripttion: Get plugin library file map in the interface info instance.
     * @Param: null
     * @Return: Reference of the _pluginLibListMap.
     */        
    inline const PluginLibFileMap& pluginLibFileMap() const {
        return _pluginLibFileMap;
    }

    /**
     * @Descripttion: Get plugin factory list in the interface info instance.
     * @Param: null
     * @Return: Reference of the _pluginFactoryList.
     */        
    inline const PluginFactoryList& pluginFactoryList() const {
        return _pluginFactoryList;
    }

    /**
     *  BEGIN: Methods of the plugin library file management, which is related with
     *  PluginLibFileMap _pluginLibFileMap
     */
    /**
     * @Descripttion: Insert a new pair of plugin name and absolute file name into the map
     * @Param: pluginName: plugin class name
     * @Param: filenname: absolute library file name of plugin.
     * @Return: CRANE_SUCC/CRANE_FAIL
     */        
    unsigned addPluginLibFile(const string& pluginName, const string& filename);

    /**
     * @Descripttion: Merge maps together.
     * @Param: : pluginLibFileMap: plugin library file map which will be merged together.  
     * @Return: CRANE_SUCC/CRANE_FAIL
     */        
    unsigned addPluginLibFile(const PluginLibFileMap& pluginLibFileMap);

    /**
     * @Descripttion: Find the library file name through the plugin implement class name.
     * @Param: : pluginName: plugin implement class name.  
     * @Return: library file name. 
     */        
    const string findPluginLibFile(const string& pluginName);

    /**
     * @Descripttion: Delete the pair of library file name and
     *               the plugin implement class name from the map.
     * @Param: pluginName: plugin implement class name.  
     * @Return: CRANE_SUCC/CRANE_FAIL
     */        
    unsigned delPluginLibFile(const string& pluginName);
    /**
     *  END: Methods of the plugin library file management, which is related with
     *  PluginLibFileMap _pluginLibFileMap
     */

    /**
     *  BEGIN: Methods of the plugin factory instance management, which is 
     *  related with PluginFactoryList   _pluginFactoryList.
     */
    /**
     * @Descripttion: Add a new plugin factory instance into the 
     *              PluginFactoryList.
     * @Param: plugin factory.  
     * @Return: CRANE_SUCC/CRANE_FAIL
     */        
    unsigned addPluginFactory(shared_ptr<IPluginFactory> pPluginFactory);

    /**
     * @Descripttion: Copy a plugin factory list into the current PluginFactoryList.
     * @Param: plugin factory list.
     * @Return: CRANE_SUCC/CRANE_FAIL
     */        
    unsigned addPluginFactory(const PluginFactoryList&);

    /**
     * @Descripttion: Get the plugin factory through plugin name.
     * @Param: pluginName: plugin class name
     * @Return: plugin factory
     */    
    shared_ptr<IPluginFactory> getPluginFactory(const string& pluginName);

    /**
     * @Descripttion: Delete a plugin factory from PluginFactoryList through
     *              the plugin name.
     * @Param: plugin name.
     * @Return: CRANE_SUCC/CRANE_FAIL
     */        
    unsigned delPluginFactory(const string& pluginName);
    /**
     *  ENG: Methods of the plugin factory instance management, which is 
     *  related with PluginFactoryList   _pluginFactoryList.
     */

    /**
     *  BEGIN: Methods of the plugin instance id management, which is 
     *  related with PluginInstanceIdMap _pluginInstanceIdMap.
     */
    /**
     * @Descripttion: Add plugin id into the list<id> of pluginName. If pluginName is not 
     *              exist in the _pluginInstanceIdMap, then insert the pair of pluginName and
     *              list<string> which contain the id.
     * @Param: pluginName: plugin class name
     * @Param: id: id of plugin instance.
     * @Return: CRANE_SUCC/CRANE_FAIL
     */            
    unsigned addPluginInstanceId(const string& pluginName, const string& id);

    /**
     * @Descripttion: Delete plugin id from the list<id> of pluginName. If the id is the last one of 
     *              the pluginName, then erase pluginName from the map.
     * @Param: pluginName: plugin class name
     * @Param: id: id of plugin instance.
     * @Return: CRANE_SUCC/CRANE_FAIL
     */    
    unsigned delPluginInstanceId(const string& pluginName, const string& id);

    /**
     * @Descripttion: Get the list of plugin id of the plugin name.
     * @Param: pluginName: plugin class name
     * @Param: id: list of plugin instance id
     * @Return: CRANE_SUCC/CRANE_FAIL
     */    
    list<string> getPluginInstanceIds(const string& pluginName) const;
    /**
     *  END: Methods of the plugin instance id management, which is 
     *  related with PluginInstanceIdMap _pluginInstanceIdMap.
     */


private:
    string              _type;              // plugin interface name

    string              _curVer;            // plugin interface version

    string              _reqFwVer;          // plugin framework version required by this interface.

    PluginFactoryList   _pluginFactoryList; // Contained the plugin factory instance of plugin interface implement.

    PluginLibFileMap    _pluginLibFileMap;  // Contained the plugin name(class name of the plugin implemention) and absolute filename of library.

    PluginInstanceIdMap _pluginInstanceIdMap; // Contained the list of plugin instance id which are belong to special plugin name(class name of the plugin implemention).
};

}

#endif
