/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-10 17:46:07
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-29 11:00:15
 */ 
#include "PluginInterfaceInfo.h"

namespace crane {

    unsigned PluginInterfaceInfo::addPluginFactory(shared_ptr<AbstractPluginFactory> pPluginFactory) {
        LOG_DEBUG("Plugin factory with Itf:{ %s }, PluginName:{ %s }, Version:{ %s }", 
                pPluginFactory->getInterfaceType().c_str(), pPluginFactory->getPluginName().c_str(),pPluginFactory->getVersion().c_str());
            
        //检查新增的插件工厂的版本和系统中当前的插件接口版本是否匹配
        const string& version = pPluginFactory->getVersion();
        if (Util::compareVersion(version, curVersion()) <= CRANE_LOWER ) {
            LOG_ERROR("Plugin version{ %s } is not acceptable.", version.c_str());
            return CRANE_FAIL;
        }

        //检查插件工厂的接口类型和系统中当前的插件接口类型是否匹配
        if (_type.compare(pPluginFactory->getInterfaceType()) != CRANE_EQUAL) {
            LOG_ERROR("Plugin interface type{ %s } is not matched with this interface[%s].", 
                        pPluginFactory->getInterfaceType().c_str(), _type.c_str());
            return CRANE_FAIL;
        }

        LOG_DEBUG("===================Add plugin factory list before=================");
        for (auto f : _pluginFactoryList) {
            LOG_DEBUG("========={ %s } { %s }========", f->getInterfaceType().c_str(), f->getPluginName().c_str());
        }

        //检查当前系统中是否已经持有了该插件的工厂
        list<shared_ptr<AbstractPluginFactory>>::iterator it;
        for (it = _pluginFactoryList.begin(); it != _pluginFactoryList.end(); ++it) {
            if (!(*it)->getPluginName().compare(pPluginFactory->getPluginName())) {
                LOG_ERROR("The PluginInterfaceInfo obj have contained this plugin{ %s }'s factory instance.", 
                            pPluginFactory->getPluginName().c_str());
                return CRANE_FAIL;
            }
        }

        LOG_INFO("Push plugin factory instance into the list.");
        _pluginFactoryList.push_back(pPluginFactory);

        LOG_DEBUG("==============Show plugin factory list=================");
        for (auto f : _pluginFactoryList) {
            LOG_DEBUG("========={ %s } { %s }========", f->getInterfaceType().c_str(), f->getPluginName().c_str());
        }
        return CRANE_SUCC;
    }

    unsigned PluginInterfaceInfo::addPluginFactory(const PluginFactoryList& insertList) {
        LOG_INFO("Move new PluginFactoryList into the _pluginFactoryList of plugin interface.");
        if (insertList.empty()) {
            LOG_ERROR("============empty!!!============");
            return CRANE_FAIL;
        }
        for (auto f : insertList) {
            LOG_DEBUG("Insert plugin factory with Intf{ %s }, PluginName{ %s }, Version{ %s } into Plugin factory list of Itf instance.", 
                        f->getInterfaceType().c_str(), f->getPluginName().c_str(), f->getVersion().c_str());
            if (CRANE_SUCC != addPluginFactory(f)) {
                continue; 
            }
        }
        return CRANE_SUCC;
    }
    
    unsigned PluginInterfaceInfo::delPluginFactory(const string& pluginName) {
        PluginFactoryList::const_iterator itr;
        for (itr = _pluginFactoryList.begin(); itr != _pluginFactoryList.end(); ++itr) {
            if ( (*itr)->getPluginName() == pluginName) {
                _pluginFactoryList.erase(itr);
                return CRANE_SUCC;
            }
        }
        return CRANE_FAIL;
    }

    //获取指定的插件类工厂 
    shared_ptr<AbstractPluginFactory> PluginInterfaceInfo::getPluginFactory(const string& pluginName) {
        PluginFactoryList::const_iterator itr;
        for (itr = _pluginFactoryList.begin(); itr != _pluginFactoryList.end(); ++itr) {
            if ((*itr)->getPluginName() == pluginName) {
                return (*itr);
            }
        }   
        return shared_ptr<AbstractPluginFactory>(nullptr);
    }

    unsigned PluginInterfaceInfo::addPluginLibFileMap(const string& pluginName, const string& filename) {
        auto ret = _pluginLibFileMap.insert(make_pair(pluginName, filename));
        if (ret.second != true) {
            LOG_ERROR("Insert pair of plugin name { %s } and absolute filename { %s } of the plugin library into the map failed.",
                                        pluginName.c_str(), filename.c_str());
            return CRANE_FAIL;
        }

        return CRANE_SUCC;
    }

    unsigned PluginInterfaceInfo::addPluginLibFileMap(const PluginLibFileMap& libfiles) {
        for (auto lib : libfiles) {
            if (CRANE_SUCC != addPluginLibFileMap(lib.first, lib.second)) { continue; }
        }
        return CRANE_SUCC;
    }

    unsigned PluginInterfaceInfo::delPluginLibFileMap(const string& pluginName) {
        LOG_ENTER();
        PluginLibFileMap::const_iterator itr = _pluginLibFileMap.find(pluginName);
        if (itr == _pluginLibFileMap.cend()) {
            LOG_ERROR("This plugin{ %s } has not existed in plugin library file map.", pluginName.c_str());
            return CRANE_SUCC;
        }
        LOG_DEBUG("===========Erase { %s %s }from _pluginLibFileMap=========", itr->first.c_str(), itr->second.c_str());
        _pluginLibFileMap.erase(itr);
        return CRANE_SUCC;        
    }

    const string PluginInterfaceInfo::findPluginLibFileMap(const string& pluginName) {
        auto itr = _pluginLibFileMap.find(pluginName);
        if (itr != _pluginLibFileMap.end()) {
            return itr->second;
        } else {
            return string();
        }
    }

}