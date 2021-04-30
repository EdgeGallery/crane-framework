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

#ifndef __CRANE_ABSPLUGINFRAME_H__
#define __CRANE_ABSPLUGINFRAME_H__

#include <string>
#include <iostream>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "craneMacro.h"
#include "Log.h"
#include "Util.h"
#include "cranePlugin.h"
#include "PluginBaseInterface.h"
#include "Wrapper.h"

using namespace std;
using namespace Pistache;

namespace NS_CRANE {
class AbstractPluginFrame {
public:
    /**
     * @Descripttion: Create crane plugin framework.
     * @Param: null 
     * @Return: AbstractPluginFrame*
     */            
    static AbstractPluginFrame* getPluginFrame();

    /**
     * @Descripttion: Release crane plugin frame.
     *              NOTE: this function is invoked for release resource
     *              during the testing.
     * @Param: null 
     * @Return: null 
     */            
    static void relPluginFrame();

    /**
     * @Descripttion: 初始化插件系统
     * @Param {type} 
     * @Return: 
     */
    virtual unsigned init(int, char**, unsigned) = 0;

    /**
     * @Descripttion:   Create Crane plugin instance without holded in
     *                  the framework.
     * @Param:          type: plugin interface type. 
     * @Param:          pluginName: plugin implement class name.
     * @Param:          id[IN/OUT]: the plugin instance id.
     * @Return:         Raw pointer of plugin instance
     */
    virtual PluginBase* create(const string& type, const string& pluginName, string& id) = 0;

    /**
     * @Descripttion:   Create crane plugin instance
     * @Param:          type: plugin interface type
     * @Param:          pluginName: plugin implement class name. 
     * @Param:          id: unique ID of plugin instance, if it is empty string, so plugin frame will generate it.
     * @Param:          desc: description of the plugin instance
     * @Return:         share_ptr<PluginBaseInterface> 
     */
    virtual shared_ptr<PluginBase> create(const string& type, const string& pluginName, string& id, const string& desc) = 0;

    /**
     * @Descripttion:   Release the shared_ptr of the plugin instance.
     *                  Release the reference to pointer of plugin instance.
     *                  Release the data of plugin instance in the registry.
     *                  Try to destory the shared_ptr of plugin instance which is
     *                  holded by framework.
     * @Param:          shared_ptr of the plugin instance. 
     * @Return:         null
     */
    virtual void release(shared_ptr<PluginBase>&& cranePlugin) = 0;

    /**
     * @Descripttion:   Destory the raw pointer of plugin instance.
     * @Param:          Raw pointer of plugin instance. 
     * @Return:         null
     */
    virtual void destory(PluginBase*) = 0;

    /**
     * @Descripttion:   Release the shared_ptr<PluginBaseInterface>
     * @Param:          id: plugin instance id 
     * @Return:         null 
     */
    virtual void destory(const string& id) = 0;

    /**
     * @Descripttion: 创建Gstreamer插件的实例
     * @Param:          gstFactoryName: gstreamer插件工厂名称
     * @Param:          name: 插件实例名称
     * @Param[out]:     pluginObj: 插件对象指针的指针
     * @Return:         CRANE_SUCC/CRANE_FAIL 
     */        
    virtual void* create(const string& gstFactoryName, const string& name) = 0;

    /**
     * @Descripttion: 销毁Gstreamer插件的实例
     * @Param: 插件对象指针的指针
     * @Return:         null 
     */        
    virtual void destory(void*) = 0;

    /**
     * @Descripttion: Load plugin dynamic library into the process, and 
     *              update the plugin interface info table, 
     *              update the plugin dynamic library table.
     * @Param: filename: Absolute filename of the library of the plugin.
     * @Param[out]: desc:  Description of the plugin.
     * @Return: NULL
     */            
    virtual unsigned load(const string& filename, PluginDesc& desc) = 0;

    /**
     * @Descripttion: Load plugin dynamic library into the process.
     * @Param: filename: Absolute filename of the library of the plugin.
     * @Return: NULL
     */            
    virtual unsigned load(const string& filename) = 0;

    /**
     * @Descripttion: Clear the information in the pluign interface info,
     *              and clear the entry in the plugin dynamic library table 
     *              which will call the the ~DlLibrary() to dlclose the 
     *              dynamic library.
     * @Param: Absolute filename of dynamic library.
     * @Return: CRANE_SUCC/CRANE_FAIL.
     */            
    virtual unsigned unload(const string& type, const string& pluginName) = 0;

    /**
     * @Descripttion: Fetch a plugin instance by id.
     * @Param: id: plugin instance id.
     * @Return: shared_ptr<PluginBaseInterface>
     */            
    virtual shared_ptr<PluginBase> instance(const string& id) = 0;

    /**
     * @Descripttion: Fetch a plugin instance by plugin interface type and plugin name.
     * @Param: itfType: plugin interface type. 
     * @Param: pluginName: plugin implemention class name. 
     * @Return: shared_ptr<PluginBaseInterface>
     */            
    virtual shared_ptr<PluginBase> instance(const string& itfType, const string& pluginName) = 0;

    /**
     * @Descripttion: Get plugin instance id by type and plugin name.
     * @Param: itfType: plugin interface type. 
     * @Param: pluginName: plugin implemention class name. 
     * @Return: plugin id
     */            
    virtual const string id(const string& itfType, const string& pluginName) const = 0;

    /**
     * @Descripttion: Create a Swappable plugin instance by plugin interface type and plugin name.
     * @Param: itfType: plugin interface type. 
     * @Param: pluginName: plugin implemention class name. 
     * @Param: id: swappable plugin id. 
     * @Param: desc:  description of the swappable plugin instance.
     * @Return: reference of Wrapper which contain shared_ptr<PluginBaseInterface>
     */            
    virtual shared_ptr<Wrapper> createSwappablePlugin(const string& itfType, const string& pluginName, string& id, const string& desc) = 0;

    /**
     * @Descripttion: Create a Swappable plugin instance by plugin instance id.
     * @Param: pluginId: plugin interface id. 
     * @Param: id: swappable plugin id.
     * @Param: desc:  description of the swappable plugin instance.
     * @Return: Wrapper object pointer which contain shared_ptr<Wrapper>
     */            
    virtual shared_ptr<Wrapper> createSwappablePlugin(const string& pluginId, string& id, const string& desc) = 0;

    /**
     * @Descripttion: Fetch a Swappable plugin instance by swappable plugin instance id.
     * @Param: id: swappable plugin id.
     * @Return: Wrapper object pointer which contain shared_ptr<Wrapper>
     */       
    virtual shared_ptr<Wrapper> fetchSwappablePlugin(const string& id) = 0;

    virtual void releaseSwappablePlugin(shared_ptr<Wrapper>&& wp) = 0;

    /**
     * @Descripttion: Swapping a Swappable plugin by the absolute ELF filename of fresh plugin instance.
     *              NOTE: wrapped plugin id will not changed, and no NEW
     *              wrapped plugin id generated for the fresh plugin instance. 
     * @Param: id: swappable plugin id.
     * @Param: freshAbsolutePluginFilename: absolute ELF filename of fresh plugin instance. 
     * @Return: CRANE_SUCC/CRANE_FAIL 
     */       
    virtual unsigned swapByFilename(const string& id, const string& freshAbsolutePluginFilename) = 0;

    /**
     * @Descripttion: Swapping a Swappable plugin by the plugin_id of fresh plugin instance.
     *              NOTE: wrapped plugin id will not changed, and no NEW
     *              wrapped plugin id generated for the fresh plugin instance. 
     * @Param: id: swappable plugin id.
     * @Param: plugin_id: plugin id of the fresh plugin instance. 
     * @Return: CRANE_SUCC/CRANE_FAIL 
     */       
    virtual unsigned swapById(const string& swappable_plugin_id, const string& plugin_id) = 0;

    virtual ~AbstractPluginFrame() { 
        std::cout<<"~AbstractPluginFrame()"<<std::endl;
    }
    
private:
    class Garbo {

        public:
            ~Garbo() {
                if (AbstractPluginFrame::_instance != nullptr) {
                    std::cout<<"~Garbo()"<<std::endl;
                    delete AbstractPluginFrame::_instance;
                }
            }                
    };

protected:
    AbstractPluginFrame();

    //_server is just used by app server thread, so it is a unique_ptr. 
    //unique_ptr<RestServer> _server{new RestServer()};    //app server instance.
    //shared_ptr<Server> _server = make_shared<Server>();

private:
    static AbstractPluginFrame* _instance;
    static Garbo                _garbo;
};

    
}

#endif
