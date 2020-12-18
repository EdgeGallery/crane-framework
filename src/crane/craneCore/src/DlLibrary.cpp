/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-29 10:07:34
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-02 16:23:53
 */
#include "DlLibrary.h"


namespace NS_CRANE {
    shared_ptr<DlLibrary> DlLibrary::Load(const string& filename) {
        if ( filename.empty() ){
            LOG_ERROR("Absolute filename of plugin is empty.");
            return shared_ptr<DlLibrary>(nullptr);
        } else {
            LOG_DEBUG("Plugin absolute filename: { %s }", filename.c_str());
        }
        void* handle = nullptr;

        handle = ::dlopen(filename.c_str(), RTLD_NOW);
        //handle = ::dlopen(filename.c_str(), RTLD_LAZY);
        if (!handle){
            const char* err = ::dlerror();
            LOG_ERROR("Failed to load library of plugin filename: { %s } with error: { %s }", filename.c_str(), err);
            return shared_ptr<DlLibrary>(nullptr);
        }

        return shared_ptr<DlLibrary>(new DlLibrary(filename, handle));
    }

    const string& DlLibrary::name() const {
        return _name;
    }

    const PluginDesc& DlLibrary::pluginDesc() const {
        return _pluginDesc;
    }

    void DlLibrary::getPluginDesc() {
        Func_Crane_Plugin_Desc getPluginDescFunc = nullptr;
        string funcName(CRANE_PLUGIN_DESC_FUNC_SYMBOL);
        getPluginDescFunc = reinterpret_cast<Func_Crane_Plugin_Desc>(symbol(funcName));
        if (getPluginDescFunc != nullptr) { 
            _pluginDesc = *getPluginDescFunc();
        } else {
            LOG_ERROR("Cannot get description of the plugin: { % }", _name.c_str());
        }
    }

    shared_ptr<AbstractPluginFactory> DlLibrary::createPluginFactory() {
        Func_Crane_Create_Plugin_Factory createFactory = nullptr;
        string funcName = string(CRANE_PLUGIN_FACTORY_FUNC_SYMBOL);
        createFactory = reinterpret_cast<Func_Crane_Create_Plugin_Factory>(symbol(funcName)); 
        if (createFactory == nullptr) {
            LOG_ERROR("Cannot get factory func of plugin: { %s }", _name.c_str());
            return shared_ptr<AbstractPluginFactory>(nullptr);
        } else {
            LOG_DEBUG("Create plugin { %s } factory instance successfully.", _name.c_str());
            return createFactory();
        }
    }

    void* DlLibrary::symbol(const string& symbolName) {
        if (!_handle){
            LOG_ERROR("Cannot inspect library symbols, library of plugin{ %s } is not loaded.", _name.c_str());
            return nullptr;
        }
        void* symbolHandle = nullptr;
        symbolHandle = (void*)(::dlsym(_handle, symbolName.c_str()));
        if (!symbolHandle){
            const char* err = ::dlerror();
            LOG_ERROR("Symbol{ %s } in library of plugin filename{ %s } is not found with error: { %s }", symbolName.c_str(), _name.c_str(), err);
            return nullptr;
        }
        return symbolHandle;
    }

    DlLibrary::DlLibrary(string name, void* handle) {
        _name = name;
        _handle = handle;
    }

    DlLibrary::~DlLibrary() {
        LOG_DEBUG("================~DlLibrary(%s) =================", _name.c_str());
        if (_handle) { 
            int ret = ::dlclose(_handle);
            if (ret) {
                const char* err = ::dlerror();
                LOG_ERROR("dlclose { %s } library with error: { %s }", _name.c_str(), err);
            }
        }
    }
        
}