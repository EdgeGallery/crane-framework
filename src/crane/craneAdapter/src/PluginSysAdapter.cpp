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

#include "PluginSysAdapter.h"

namespace NS_CRANE {
    PluginSysAdapter::PluginSysAdapter() {
        _config = make_shared<Config>();
    }

    unsigned PluginSysAdapter::init(int argc, char** argv, unsigned mode) {
        //Intialize the log parameter.
        Log::getInstance()->setLevel(Log::getInstance()->cvtLevel(_config->logLevel));
        Log::getInstance()->setTarget(Log::getInstance()->cvtTarget(_config->logTarget));

        //Print the crane system config.
        _config->showConfigInfo();

        LOG_INFO("Crane is initializing...");

        //Initialize the Crane and Gstreamer system.
        switch (mode) {
            case CRANE_CRN:
                craneInit(argc, argv);
                LOG_INFO("Initialized crane plugin system only.");
                break;
            case CRANE_GST:
                gstInit(argc, argv);
                LOG_INFO("Initialized gstreamer plugin system only.");
                break;
            case CRANE_ALL:
                craneInit(argc, argv);
                gstInit(argc, argv);
                LOG_INFO("Initialized crane and gstreamer plugin system.");
                break;
            default:
                LOG_ERROR("Invalid initialized mode[%u]", mode);
                return CRANE_FAIL;
        }

        return CRANE_SUCC;
    }

    PluginBase* PluginSysAdapter::create(const string& itfType, 
                                        const string& pluginName, 
                                        string& id) {
        LOG_INFO("Creating plugin[%s] of Itf[%s]...", pluginName.c_str(), itfType.c_str());
        
        lock_guard<mutex> lock(_mtx);
        
        shared_ptr<PluginInterfaceInfo> spItfInfo = findPluginItfInfo(itfType);
        if (!spItfInfo) {
            //该接口类型再CRANE系统中不存在；
            LOG_ERROR("Plugin interface type[%s] is not exist in Crane Registry.", itfType.c_str());
            return nullptr;
        }

        const string absoluteLibFilename = spItfInfo->findPluginLibFile(pluginName);
        if (absoluteLibFilename.empty()) {
            LOG_ERROR("Plugin { %s } is not in the registry.", pluginName.c_str());
            return nullptr;
        }

        shared_ptr<IPluginFactory> factory = spItfInfo->getPluginFactory(pluginName);
        if (!factory) {
            LOG_INFO("Library file of plugin [%s] is not loaded.", pluginName.c_str());

            //Create a new ItfInfo and move the FactoryList into the current ItfInfo. 
            shared_ptr<PluginInterfaceInfo> newItfInfo = createItfInfo(absoluteLibFilename);
            if (!newItfInfo) {
                LOG_ERROR("Create a new itfInfo instance with library filename { %s } failed.", absoluteLibFilename.c_str());
                return nullptr;
            }
            spItfInfo->addPluginFactory(newItfInfo->pluginFactoryList());

            //After load the plugin library, get factory of plugin again.
            factory = spItfInfo->getPluginFactory(pluginName);
            if (!factory) {
                LOG_ERROR("Get factory of PluginName: { %s } failed", pluginName.c_str());
                return nullptr;
            }
        }

        PluginBase* plugin = factory->create();
        if (plugin == nullptr) { return nullptr; }

        // Set plugin instance id.
        if(id.empty()) { id = Util::uuid(); }
        plugin->id(id);

        if (factory->isAllowInit()) { 
            if (CRANE_SUCC != plugin->init()) {
                LOG_ERROR("init() is failed");
                return nullptr;
            }
        }

        LOG_INFO("Create plugin[%s] of Itf[%s] successfully", pluginName.c_str(), itfType.c_str());
        return plugin;
    }

    shared_ptr<PluginBase> PluginSysAdapter::create(const string& type, 
                                                    const string& pluginName, 
                                                    string& id, 
                                                    const string& description) {
        unused(description);
                                                                
        if (id.empty()) { id = Util::uuid(); }

        PluginBase* plugin = create(type, pluginName, id);
        if (plugin == nullptr) {
            return shared_ptr<PluginBase>(nullptr);
        }
        shared_ptr<PluginBase> plugin_shared_ptr;
        plugin_shared_ptr.reset(plugin);
         
        if (CRANE_SUCC != addPluginInstance(id, plugin_shared_ptr, type, pluginName)) {
            // ATTENTION!!!
            // plugin_shared_ptr will release when leave the scope of this function.
            // so raw pointer of plugin will be delete.
            return shared_ptr<PluginBase>(nullptr);
        }

        return plugin_shared_ptr;
    }

    void PluginSysAdapter::release(shared_ptr<PluginBase>&& cranePlugin) {
        string id = cranePlugin->id();

        // Release the reference to the pointer of the plugin.
        cranePlugin.reset();

        // Release data in the registry and try to release instance of the plugin.
        relPluginInstance(id);
    }

    void PluginSysAdapter::destory(PluginBase* cranePlugin) {
        if (cranePlugin != nullptr) {
            delete cranePlugin;
        }
    }

    void PluginSysAdapter::destory(const string& id) {
        relPluginInstance(id);
    }

    void* PluginSysAdapter::create(const string& gstFactoryName, const string& name) {
        void* plugin;
        plugin = gstCreate(gstFactoryName, name); 
        return plugin;
    }

    void PluginSysAdapter::destory(void* gstPlugin) {
        if (gstPlugin != nullptr) {
            gstDestory(gstPlugin);
        }
    }
    
    unsigned PluginSysAdapter::load(const string& filename, PluginDesc& desc) {
        unsigned ret;
        {
            lock_guard<mutex> lock(_mtx);
            shared_ptr<PluginInterfaceInfo> newItfInfo = createItfInfo(filename, desc);
            const string type = newItfInfo->type();
            ret = registry_(newItfInfo);
        }
        if (CRANE_SUCC != ret) {
            LOG_ERROR("Load library: { %s } into registry failed.", filename.c_str());
        }
        return ret;
    }

    unsigned PluginSysAdapter::load(const string& filename) {
        PluginDesc desc{};
        return load(filename, desc);
    }

    unsigned PluginSysAdapter::unload(const string& type, const string& pluginName) {
        lock_guard<mutex> lock(_mtx);
        return clearPlugin(type, pluginName);
    }

    shared_ptr<PluginBase> PluginSysAdapter::instance(const string& id) {
        return getPluginInstance(id);
    }

    shared_ptr<PluginBase> PluginSysAdapter::instance(const string& itfType, const string& pluginName) {
        return getPluginInstance(itfType, pluginName);
    }

    const string PluginSysAdapter::id(const string& itfType, const string& pluginName) const {
        return getPluginId(itfType, pluginName);
    }

    shared_ptr<Wrapper> PluginSysAdapter::createSwappablePlugin(
        const string& itfType, 
        const string& pluginName, 
        string& id, 
        const string& desc) {
        // Generate the plugin instance id.
        string pluginId = Util::uuid();

        // Get plugin instance which has been holded by Registry's _pluginInstanceMap.
        shared_ptr<PluginBase> p = create(itfType, pluginName, pluginId, desc);

        // Make shared_ptr<Wrapper> and set wrapperable plugin id.
        shared_ptr<Wrapper> wp = make_shared<Wrapper>(p);
        if (id.empty()) { id = Util::uuid(); }
        wp->id(id);

        addPluginSwappableInstance(id, wp, pluginId, desc);
        return getPluginSwappableInstance(id);
    }

    shared_ptr<Wrapper> PluginSysAdapter::createSwappablePlugin(
        const string& pluginId, 
        string& id, 
        const string& desc) {
        auto p = getPluginInstance(pluginId);
        if (p) {
            return shared_ptr<Wrapper>(nullptr);
        }

        shared_ptr<Wrapper> wp = make_shared<Wrapper>(p);
        if (id.empty()) { id = Util::uuid(); }
        wp->id(id);

        addPluginSwappableInstance(id, wp, pluginId, desc);
        return getPluginSwappableInstance(id);
    }
    
    shared_ptr<Wrapper> PluginSysAdapter::fetchSwappablePlugin(const string& id) {
        return getPluginSwappableInstance(id);
    }

    void PluginSysAdapter::releaseSwappablePlugin(shared_ptr<Wrapper>&& wp) {
        string id = wp->id();
        wp.reset();
        relPluginSwappableInstance(id);
    }

    unsigned PluginSysAdapter::swapByFilename(const string& id, const string& filename) {
        unsigned ret = CRANE_FAIL; 
        PluginDesc desc;
        ret = load(filename, desc);
        if (CRANE_SUCC != ret) { return ret; }

        string newPluginInstanceId {};
        shared_ptr<PluginBase> freshPlugin = create(desc.itfType, desc.pluginName,  newPluginInstanceId, "");
        shared_ptr<Wrapper> stableWrappedPlugin = getPluginSwappableInstance(id);

        ret = _handover(stableWrappedPlugin, freshPlugin);
        if (CRANE_SUCC != ret) { return ret; }
        
        return CRANE_SUCC;
    }
    unsigned PluginSysAdapter::swapById(const string& swappable_plugin_id, const string& plugin_id) {
        shared_ptr<PluginBase> freshPlugin = instance(plugin_id);
        shared_ptr<Wrapper> stableWrappedPlugin = getPluginSwappableInstance(swappable_plugin_id);
        unsigned ret = _handover(stableWrappedPlugin, freshPlugin);
        if (CRANE_SUCC != ret) { return ret; }
       
        return CRANE_SUCC;
    }

    unsigned PluginSysAdapter::_handover(shared_ptr<Wrapper> stableWrappedPlugin, shared_ptr<PluginBase> freshPlugin) {
        // lock the wrapper plugin.
        lock_guard<mutex> lock(stableWrappedPlugin->_mtx);

        // Stop stable plugin...
        stableWrappedPlugin->_plugin->stop();

        // swap_down stale plugin...
        dynamic_pointer_cast<PluginSwappable>(stableWrappedPlugin->_plugin)->swap_down(stableWrappedPlugin->_plugin, freshPlugin);

        // swap_up fresh plugin...
        dynamic_pointer_cast<PluginSwappable>(freshPlugin)->swap_up(stableWrappedPlugin->_plugin, freshPlugin);

        // Start fresh plugin...
        freshPlugin->start();

        stableWrappedPlugin->_p(freshPlugin);
        return CRANE_SUCC;        
    }

    PluginSysAdapter::~PluginSysAdapter() {
        cout<<"~PluginSysAdapter()"<<endl;
    }
}
