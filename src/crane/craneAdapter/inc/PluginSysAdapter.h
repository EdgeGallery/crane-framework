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

#ifndef __CRANE_PLUGINSYSADAPTER_H__
#define __CRANE_PLUGINSYSADAPTER_H__

#include <cstdio>
#include <string>
#include <memory>
#include <iostream>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "craneMacro.h"
#include "Config.h"
#include "Util.h"
#include "Log.h"
#include "AbsPluginFrame.h"
#include "GstFacade.h"
#include "CraneRegistry.h"

#include "Wrapper.h"

#include "CraneMsgCenterInterface.h"

#ifdef BUILD_IMG 
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "Data.h"
#include "ImgData.h"
#include "restServerProcess.h"
#endif

using namespace std;

namespace NS_CRANE {

class PluginSysAdapter : public AbstractPluginFrame,  private CraneRegistry, private GstFacade {
friend class AbstractPluginFrame;

public:
    /**
     * @Descripttion: initialize the framework by parameter.
     * @Param: mode: intitialize either Crane or Gstreamer or both. 
     * @Return: CRANE_SUCC/CRANE_FAIL 
     */            
    unsigned init(int, char**, unsigned mode) override;

    /**
     * @Descripttion: Create a plugin instance though the interface type and plugin implemention class name. 
     * If the library file of the plugin implemention class has not loaded, Crane will load it depend on the 
     * absolute file name of library contained in the registry.
     * @Param:  type: Interface type of plugin, which is same as the pure virtual class name of the plugin.
     *          pluginName: Plugin class name which implement the pure virtual class name of the plugin.
     *          id[IN/OUT]: plugin instance id.                      
     * @Return: pointer of the instance of the plugin.
     */            
    PluginBase* create(const string& type, const string& pluginName, 
                    string& id) override;
    /**
     * @Descripttion:   Create crane plugin instance
     * @Param:          type: plugin interface type
     * @Param:          pluginName: plugin implement class name. 
     * @Param:          id: unique ID of plugin instance, if it is empty string, 
     *                  so plugin frame will generate it.
     * @Param:          desc: description of the plugin instance
     * @Return:         share_ptr<PluginBaseInterface> 
     */
    shared_ptr<PluginBase> create(const string& type, const string& pluginName, 
                            string& id, const string& description) override;

    /**
     * @Descripttion: Release the plugin throuth the raw point of plugin instance.
     * @Param: the pointer of the plugin instance.
     * @Return: null
     */            
    void destory(PluginBase*) override;

    /**
     * @Descripttion:   Release the shared_ptr<PluginBaseInterface> through the plugin id.
     * @Param:          id: plugin instance id 
     * @Return:         null 
     */
    void destory(const string& id) override;

    /**
     * @Descripttion: Create Gestreamer plugin, this method just wrap the function of Gstreamer. 
     * @Param: gstFactoryName: the name of the factory of the plugin 
     * @Param: name: the name of the plugin 
     * @Return: pointer of the instance of the plugin. 
     */            
    void* create(const string& gstFactoryName, const string& name) override;

    /**
     * @Descripttion: Realse the Gestreamer plugin, this method just wrap the function of Gstreamer. 
     * @Param: the pointer of the plugin 
     * @Return: null 
     */            
    void destory(void*) override;

    /**
     * @Descripttion: Load the plugin throuth the absolute library file name. 
     *                This method is used after Crane has initialized.
     * @Param: filename: Absolute filename of the library of the plugin.
     * @Param[out]: desc:  Description of the plugin.
     * @Return: CRANE_SUCC/CRANE_FAIL 
     */            
    unsigned load(const string& filename, PluginDesc& desc) override;

    /**
     * @Descripttion: Load the plugin throuth the absolute library file name. 
     *                This method is used after Crane has initialized.
     * @Param: filename: absolute library file name.
     * @Return: CRANE_SUCC/CRANE_FAIL 
     */            
    unsigned load(const string& filename) override; 

    /**
     * @Descripttion: Unload the plugin library from virtual memory space of the process.
     * @Param:  type: Interface type of plugin, which is same as the pure virtual class name of the plugin.
     *          pluginName: Plugin class name which implement the pure virtual class name of the plugin.
     *          description: Description of this instance of the plugin.                      
     * @Return: null
     */            
    void unload(const string& type, const string& pluginName) override;

    /**
     * @Descripttion: Fetch a plugin instance by id.
     * @Param: id: plugin instance id.
     * @Return: shared_ptr<PluginBaseInterface>
     */            
    shared_ptr<PluginBase> instance(const string& id) override;

    /**
     * @Descripttion: Fetch a plugin instance by plugin interface type and plugin name.
     * @Param: itfType: plugin interface type. 
     * @Param: pluginName: plugin implemention class name. 
     * @Return: shared_ptr<PluginBaseInterface>
     */            
    shared_ptr<PluginBase> instance(const string& itfType, 
                                const string& pluginName) override; 

    /**
     * @Descripttion: Get plugin instance id by type and plugin name.
     * @Param: itfType: plugin interface type. 
     * @Param: pluginName: plugin implemention class name. 
     * @Return: plugin id
     */            
    const string id(const string& itfType, const string& pluginName) const override;

    /**
     * @Descripttion: Create a Swappable plugin instance by plugin interface type and plugin name.
     * @Param: itfType: plugin interface type. 
     * @Param: pluginName: plugin implemention class name. 
     * @Param: id[IN/OUT]: swappable plugin id. 
     * @Param: desc:  description of the swappable plugin instance.
     * @Return: reference of Wrapper which contain shared_ptr<PluginBaseInterface>
     */            
    shared_ptr<Wrapper> createSwappablePlugin(const string& itfType, 
                                            const string& pluginName, 
                                            string& id, 
                                            const string& desc) override;

    /**
     * @Descripttion: Create a Swappable plugin instance by plugin instance id.
     * @Param: pluginId: plugin interface id. 
     * @Param: id[IN/OUT]: swappable plugin id.
     * @Param: desc:  description of the swappable plugin instance.
     * @Return: Wrapper object pointer which contain shared_ptr<Wrapper>
     */            
    shared_ptr<Wrapper> createSwappablePlugin(const string& pluginId, 
                                            string& id, 
                                            const string& desc) override;
    /**
     * @Descripttion: Fetch a Swappable plugin instance by swappable plugin instance id.
     * @Param: id: swappable plugin id.
     * @Return: Wrapper object pointer which contain shared_ptr<Wrapper>
     */   
    shared_ptr<Wrapper> fetchSwappablePlugin(const string& id) override;

    /**
     * @Descripttion: Swapping a Swappable plugin by the absolute ELF filename of fresh plugin instance.
     *              NOTE: wrapped plugin id will not changed, and no NEW
     *              wrapped plugin id generated for the fresh plugin instance. 
     * @Param: id: swappable plugin id.
     * @Param: freshAbsolutePluginFilename: absolute ELF filename of fresh plugin instance. 
     * @Return: CRANE_SUCC/CRANE_FAIL 
     */              
    unsigned swapByFilename(const string& id, const string& freshAbsolutePluginFilename) override;

    /**
     * @Descripttion: Swapping a Swappable plugin by the plugin_id of fresh plugin instance.
     *              NOTE: wrapped plugin id will not changed, and no NEW
     *              wrapped plugin id generated for the fresh plugin instance. 
     * @Param: id: swappable plugin id.
     * @Param: plugin_id: plugin id of the fresh plugin instance. 
     * @Return: CRANE_SUCC/CRANE_FAIL 
     */
    unsigned swapById(const string& swappable_plugin_id, const string& plugin_id) override;

public:
    ~PluginSysAdapter() override;

protected:
    PluginSysAdapter();

private:

    /**
     * @Descripttion: Handover origin wrapped plugin by the fresh plugin instance.
     * @Param: statlePlugin: stale wrapped plugin 
     *                  which's plugin instance will be handed down. 
     * @Param: freshPlugin: plugin instance which will be handed up.
     * @Return: CRANE_SUCC/CRANE_FAIL 
     */            
    unsigned _handover(shared_ptr<Wrapper> stalePlugin, 
                    shared_ptr<PluginBase> freshPlugin);

private:
    shared_ptr<Config>          _config;

    mutex                       _mtx;
};
}

#endif
