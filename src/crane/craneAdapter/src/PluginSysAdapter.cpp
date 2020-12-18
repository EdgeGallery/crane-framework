/*
 * @Descripttion: 插件框架适配器
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-09 16:53:13
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-04 17:37:42
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

    PluginBaseInterface* PluginSysAdapter::create(const string& itfType, const string& pluginName, const string& name) {
        LOG_INFO("Creating plugin[%s] of Itf[%s]...", pluginName.c_str(), itfType.c_str());

        unused(name);

        lock_guard<mutex> lock(_mtx);
        
        shared_ptr<PluginInterfaceInfo> spItfInfo = getPluginItfInfo(itfType);
        if (!spItfInfo) {
            //该接口类型再CRANE系统中不存在；
            LOG_ERROR("Plugin interface type[%s] is not exist in Crane Registry.", itfType.c_str());
            return nullptr;
        }

        const string absoluteLibFilename = spItfInfo->findPluginLibFileMap(pluginName);
        if (absoluteLibFilename.empty()) {
            LOG_ERROR("Plugin { %s } is not in the registry.", pluginName.c_str());
            return nullptr;
        }

        shared_ptr<AbstractPluginFactory> factory = spItfInfo->getPluginFactory(pluginName);
        if (!factory) {
            LOG_INFO("Library file of plugin [%s] is not loaded.", pluginName.c_str());

            //Create a new ItfInfo and move the FactoryList into the current ItfInfo. 
            shared_ptr<PluginInterfaceInfo> newItfInfo = createItfInfo(absoluteLibFilename);
            if (!newItfInfo) {
                LOG_ERROR("Create a new itfInfo instance with library filename { %s } failed.", absoluteLibFilename.c_str());
                return nullptr;
            }
            spItfInfo->addPluginFactory(newItfInfo->getPluginFactoryList());

            //After load the plugin library, get factory of plugin again.
            factory = spItfInfo->getPluginFactory(pluginName);
            if (!factory) {
                LOG_ERROR("Get factory of PluginName: { %s } failed", pluginName.c_str());
                return nullptr;
            }
        }

        PluginBaseInterface* plugin = factory->create();
        if (plugin == nullptr) { return nullptr; }

        if (factory->isAllowInit()) { plugin->init(); }

        LOG_INFO("Create plugin[%s] of Itf[%s] successfully", pluginName.c_str(), itfType.c_str());
        return plugin;
    }

    void PluginSysAdapter::destory(PluginBaseInterface* cranePlugin) {
        if (cranePlugin != nullptr) {
            delete cranePlugin;
        }
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

    unsigned PluginSysAdapter::load(const string& filename) {
        unsigned ret;
        {
            lock_guard<mutex> lock(_mtx);
            shared_ptr<PluginInterfaceInfo> newItfInfo = createItfInfo(filename);
            const string type = newItfInfo->type();
            ret = registry_(newItfInfo);
        }
        if (CRANE_SUCC != ret) {
            LOG_ERROR("Load library: { %s } into registry failed.", filename.c_str());
        }
        return ret;
    }

    void PluginSysAdapter::unload(const string& type, const string& pluginName) {
        lock_guard<mutex> lock(_mtx);
        clearPlugin(type, pluginName);
        return;
    }

    PluginSysAdapter::~PluginSysAdapter() {
        cout<<"~PluginSysAdapter()"<<endl;
        //if (_mepMediator != nullptr) {delete _mepMediator;}
    }
}