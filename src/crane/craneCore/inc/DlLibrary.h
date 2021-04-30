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

#ifndef __CRANE_DLLIBRARY_H__
#define __CRANE_DLLIBRARY_H__


#include <dlfcn.h>
#include <cstdio>
#include <string>
#include <memory>

#include "craneMacro.h"
#include "Log.h"
#include "cranePlugin.h"
#include "AbsPluginFactory.h"

using namespace std;

namespace NS_CRANE {
class DlLibrary : public enable_shared_from_this<DlLibrary> {
    public:
        /**
         * @Descripttion: Load the library file of the plugin into the current process.
         * @Param: filename: absolute filename of library file.
         * @Return: 
         */        
        static unique_ptr<DlLibrary> Load(const string& filename);

        /**
         * @Descripttion: Get handle of symbol in the library.
         * @Param: symbolName: name of the symbol.
         * @Return: handle of the symbol
         */        
        void* symbol(const string& symbolName);

        /**
         * @Descripttion: Invoke the function in plugin library,
         *              initialize the plugin description data and
         *              return the result.
         * @Param: null
         * @Return: PluginDesc
         */        
        const PluginDesc& pluginDesc();

        /**
         * @Descripttion: Get absolute filename of the plugin's library
         * @Param: null
         * @Return: 
         */        
        inline const string& name() const {
            return _name;
        }
        
        /**
         * @Descripttion: Create and return the handle of instance of the plugin factory.
         * @Param: null
         * @Return: handle of instance of the plugin factory
         */        
        shared_ptr<IPluginFactory> createPluginFactory();

    private:
        DlLibrary() = delete;

        DlLibrary(const DlLibrary&) = delete;

        DlLibrary& operator=(const DlLibrary&) = delete;

        DlLibrary(const string& name, void* handle) : _name(name), _handle(handle) {};

    public:
        ~DlLibrary();

    private:
        using Func_Crane_Plugin_Desc = PluginDesc* (*)();

        using Func_Crane_Create_Plugin_Factory = shared_ptr<IPluginFactory> (*)();

        string          _name;                      //library file name of the plugin which is not included libcraneplugin prefix and .so suffix.

        PluginDesc      _pluginDesc;                //description of library of the plugin.

        void*           _handle = nullptr;          //handle of the plugin library.
};

}

#endif
