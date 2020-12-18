/*
 * @Descripttion: Crane插件注册中心
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-09 16:15:57
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-29 16:43:32
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

using namespace std;
using namespace rapidjson;

namespace NS_CRANE {

class CraneRegistry/* : public AbsRegistry */{
    public:
        CraneRegistry() = default;

        ~CraneRegistry() {
            cout<<"~CraneRegistry()"<<endl;
            //TAKE CARE: itfMap must release before libMap, because itfMap contain the pointer of the plugin factory.
            //When I release(::dlclose) the library first and then release pointer of the plugin factroy, process can 
            //not find memory of plugin factory.
            _pluginItfMap.clear();
            _pluginLibMap.clear();
        }

        /**
         * @Descripttion: Initialize the crane system.
         * @Param: 
         * @Return:  
         */        
        unsigned craneInit(int argc, char** argv);

        shared_ptr<PluginInterfaceInfo> getPluginItfInfo(const string&);
        
        shared_ptr<PluginInterfaceInfo> createItfInfo(const string& absoluteFileName);

        unsigned clearPlugin(const string& type, const string& pluginName);

    protected:
        unsigned registry_(shared_ptr<PluginInterfaceInfo>);

    private:
        using PluginInterfaceMap = map<string /*interface name*/, shared_ptr<PluginInterfaceInfo>>;
        PluginInterfaceMap _pluginItfMap; //map contain pair of interface name and interface instance.

        using PluginImplementMap = map<string /*libName*/,  shared_ptr<DlLibrary>>; //libName is ABCD which is extracted from libcranepluginABCD.so
        PluginImplementMap _pluginLibMap; //map contain pair of library file name and library handle.
        
        static const string   _CACHE_FILE;
        static const string   _CACHE_FILE_TMP;

        enum class _InitMode {
            LIB_FILE   = 0,
            CACHE_FILE,
        };

        mutex       _mtx;

        list<string> _dlFiles; //library files.

        unsigned _addPluginLibMap(shared_ptr<DlLibrary>);

        /**
         * @Descripttion: Remove the library from _pluginLibMap and close the handle of the library.
         * @Param:  filename: Absolate filename of the plugin library
         * @Return: CRANE_SUCC/CRANE_FAIL 
         */        
        unsigned _delPluginLibMap(const string& filename);

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