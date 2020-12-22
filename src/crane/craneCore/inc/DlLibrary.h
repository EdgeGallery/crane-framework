/*
 * @Descripttion: Plugin library class
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-12 16:11:31
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-10 11:55:25
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
        //static shared_ptr<DlLibrary> Load(const string& filename);
        static unique_ptr<DlLibrary> Load(const string& filename);

        /**
         * @Descripttion: Get handle of symbol in the library.
         * @Param: symbolName: name of the symbol.
         * @Return: handle of the symbol
         */        
        void* symbol(const string& symbolName);

        /**
         * @Descripttion: Invoke the function in plugin library and initialize the plugin description data.
         * @Param: null
         * @Return: null
         */        
        void getPluginDesc(); 

        /**
         * @Descripttion: Get absolute filename of the plugin's library
         * @Param: null
         * @Return: 
         */        
        inline const string& name() const {
            return _name;
        }
        
        /**
         * @Descripttion: Get plugin description information.
         * @Param: null
         * @Return: 
         */        
        inline const PluginDesc& pluginDesc() const {
            return _pluginDesc;
        }

        /**
         * @Descripttion: Create and return the handle of instance of the plugin factory.
         * @Param: null
         * @Return: handle of instance of the plugin factory
         */        
        shared_ptr<AbstractPluginFactory> createPluginFactory();

    private:
        DlLibrary() = delete;

        DlLibrary(const DlLibrary&) = delete;

        DlLibrary& operator=(const DlLibrary&) = delete;

        DlLibrary(const string& name, void* handle) : _name(name), _handle(handle) {};

    public:
        ~DlLibrary();

    private:
        using Func_Crane_Plugin_Desc = PluginDesc* (*)();

        using Func_Crane_Create_Plugin_Factory = shared_ptr<AbstractPluginFactory> (*)();

        string          _name;                      //library file name of the plugin which is not included libcraneplugin prefix and .so suffix.

        PluginDesc      _pluginDesc;                //description of library of the plugin.

        void*           _handle = nullptr;          //handle of the plugin library.
};

}

#endif