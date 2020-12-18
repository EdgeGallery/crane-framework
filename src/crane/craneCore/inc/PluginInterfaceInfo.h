/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-10 17:46:26
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-27 10:56:56
 */ 
#ifndef __CRANE_PLUGININTERFACEINFO_H__
#define __CRANE_PLUGININTERFACEINFO_H__

//#define __x86_64__

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
///////////////////////////////////////////////////////////////////////////////////
/// \brief  实现InterfaceInfo接口。
///         该类的一个实例对应一个插件接口，list中存储了该插件接口的多个插件实现类的工厂类
///////////////////////////////////////////////////////////////////////////////////
class PluginInterfaceInfo /*: public AbstractInterfaceInfo */{
    public:
        using PluginFactoryList = list<shared_ptr<AbstractPluginFactory>>;

        using PluginLibFileMap = map<string/*pluginName(class name of the plugin)*/, string /*absolute library filename*/>;

        PluginInterfaceInfo(string type, string curVer, string reqFwVer) 
                            : _type(type), _curVer(curVer), _reqFwVer(reqFwVer) {}

        /**
         * @Descripttion: Get plugin interface name which is the key of plugin interface in the registry.
         * @Param: null
         * @Return: plugin interface name.
         */        
        const string& type() const {
            return _type;
        }

        /**
         * @Descripttion: Get the version of the interface of the plugin.
         * @Param: null
         * @Return: the version of tht interface.
         */        
        const string& curVersion() const {
            return _curVer;
        }

        /**
         * @Descripttion: Get the required the version of the Crane plugin framework for this plugin interface.
         * @Param: null
         * @Return: Required the version of Crane plugin framework.
         */        
        const string& requireFwVer() const {
            return _reqFwVer;
        }

        unsigned validateVersion(const string interfaceType) = delete;

        const PluginLibFileMap& getPluginLibFileMap() const {
            return _pluginLibFileMap;
        }

        const PluginFactoryList& getPluginFactoryList() const {
            return _pluginFactoryList;
        }

        unsigned addPluginLibFileMap(const string& pluginName, const string& filename);

        unsigned addPluginLibFileMap(const PluginLibFileMap&);

        const string findPluginLibFileMap(const string& pluginName);

        unsigned delPluginLibFileMap(const string& pluginName);

        unsigned addPluginFactory(shared_ptr<AbstractPluginFactory> pPluginFactory);

        unsigned addPluginFactory(const PluginFactoryList&);

        unsigned delPluginFactory(const string& pluginName);

        shared_ptr<AbstractPluginFactory> getPluginFactory(const string& pluginName);

    private:
        string              _type;

        string              _curVer;

        string              _reqFwVer;

        PluginFactoryList   _pluginFactoryList; //Contained the plugin factory instance of plugin interface implement.

        PluginLibFileMap    _pluginLibFileMap;  //Contained the plugin name(class name of the plugin implemention) and absolute filename of library. 
};

}

#endif