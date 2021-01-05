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

#include "DlLibrary.h"


namespace NS_CRANE {
    //shared_ptr<DlLibrary> DlLibrary::Load(const string& filename) {
    unique_ptr<DlLibrary> DlLibrary::Load(const string& filename) {
        if ( filename.empty() ){
            LOG_ERROR("Absolute filename of plugin is empty.");
            //return shared_ptr<DlLibrary>(nullptr);
            return unique_ptr<DlLibrary>(nullptr);
        } else {
            LOG_DEBUG("Plugin absolute filename: { %s }", filename.c_str());
        }

        void* handle = ::dlopen(filename.c_str(), RTLD_NOW);
        //handle = ::dlopen(filename.c_str(), RTLD_LAZY);
        if (!handle){
            const char* err = ::dlerror();
            LOG_ERROR("Failed to load library of plugin filename: { %s } with error: { %s }", filename.c_str(), err);
            return unique_ptr<DlLibrary>(nullptr);
        }

        //return shared_ptr<DlLibrary>(new DlLibrary(filename, handle));
        return unique_ptr<DlLibrary>(new DlLibrary(filename, handle));
    }

    void DlLibrary::getPluginDesc() {
        string funcName(CRANE_PLUGIN_DESC_FUNC_SYMBOL);
        Func_Crane_Plugin_Desc getPluginDescFunc = reinterpret_cast<Func_Crane_Plugin_Desc>(symbol(funcName));
        if (getPluginDescFunc != nullptr) { 
            _pluginDesc = *getPluginDescFunc();
        } else {
            LOG_ERROR("Cannot get description of the plugin: { % }", _name.c_str());
        }
    }

    shared_ptr<AbstractPluginFactory> DlLibrary::createPluginFactory() {
        string funcName = string(CRANE_PLUGIN_FACTORY_FUNC_SYMBOL);
        Func_Crane_Create_Plugin_Factory createFactory = reinterpret_cast<Func_Crane_Create_Plugin_Factory>(symbol(funcName));
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
        void* symbolHandle = (void*)(::dlsym(_handle, symbolName.c_str()));
        if (!symbolHandle){
            const char* err = ::dlerror();
            LOG_ERROR("Symbol{ %s } in library of plugin filename{ %s } is not found with error: { %s }", symbolName.c_str(), _name.c_str(), err);
            return nullptr;
        }
        return symbolHandle;
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
