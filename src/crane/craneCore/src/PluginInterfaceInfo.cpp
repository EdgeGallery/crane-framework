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

#include "PluginInterfaceInfo.h"

namespace NS_CRANE {

    unsigned PluginInterfaceInfo::addPluginFactory(shared_ptr<AbstractPluginFactory> pPluginFactory) {
        LOG_DEBUG("Plugin factory with Itf:{ %s }, PluginName:{ %s }, Version:{ %s }", 
                pPluginFactory->getInterfaceType().c_str(), pPluginFactory->getPluginName().c_str(),pPluginFactory->getVersion().c_str());
            
        const string& version = pPluginFactory->getVersion();
        if (Util::compareVersion(version, curVersion()) <= CRANE_LOWER ) {
            LOG_ERROR("Plugin version{ %s } is not acceptable.", version.c_str());
            return CRANE_FAIL;
        }

        if (_type.compare(pPluginFactory->getInterfaceType()) != CRANE_EQUAL) {
            LOG_ERROR("Plugin interface type{ %s } is not matched with this interface[%s].", 
                        pPluginFactory->getInterfaceType().c_str(), _type.c_str());
            return CRANE_FAIL;
        }

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
        for (const auto& f : _pluginFactoryList) {
            LOG_DEBUG("========={ %s } { %s }========", f->getInterfaceType().c_str(), f->getPluginName().c_str());
        }
        return CRANE_SUCC;
    }

    unsigned PluginInterfaceInfo::addPluginFactory(const PluginFactoryList& insertList) {
        LOG_INFO("Move new PluginFactoryList into the _pluginFactoryList of plugin interface.");
        if (insertList.empty()) {
            return CRANE_FAIL;
        }
        
        for (auto f : insertList) {
            LOG_DEBUG("Insert plugin factory with Intf{ %s }, PluginName{ %s }, Version{ %s } into Plugin factory list of Itf instance.", 
                        f->getInterfaceType().c_str(), f->getPluginName().c_str(), f->getVersion().c_str());
            if (CRANE_SUCC != addPluginFactory(f)) {
                continue; 
            }
        }
        /*
        for (auto itr = std::make_move_iterator(insertList.begin()); itr != std::make_move_iterator(insertList.end());) {
            unique_ptr<int> i = unique_ptr<int>(new int(1));
            
            //unique_ptr<AbstractPluginFactory> tmp = std::move(*itr); 
            //unique_ptr<AbstractPluginFactory> tmp = std::move(*itr);
            addPluginFactory(*itr);
            
        }
        */
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
    shared_ptr<AbstractPluginFactory> PluginInterfaceInfo::pluginFactory(const string& pluginName) {
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
        _pluginLibFileMap.erase(itr);
        return CRANE_SUCC;        
    }

    string PluginInterfaceInfo::findPluginLibFileMap(const string& pluginName) {
        auto itr = _pluginLibFileMap.find(pluginName);
        if (itr != _pluginLibFileMap.end()) {
            return itr->second;
        } else {
            return string();
        }
    }
    // #if 0 // dongyin 2-27
    unsigned PluginInterfaceInfo::addPluginInstanceId(const string& pluginName, const string& uuid) {
        PluginInstanceIdMap::iterator itr;
        itr = _pluginInstanceIdMap.find(pluginName);
        if (itr == _pluginInstanceIdMap.end()) {
            auto ret = _pluginInstanceIdMap.insert(make_pair(pluginName, list<string>{}));
            if (!ret.second) { return CRANE_FAIL; }
            itr = ret.first;
        }

        itr->second.push_back(uuid);
        return CRANE_SUCC;
    }

    unsigned PluginInterfaceInfo::delPluginInstanceId(const string& pluginName, const string& uuid) {
        PluginInstanceIdMap::iterator itr;
        itr = _pluginInstanceIdMap.find(pluginName);
        if (itr == _pluginInstanceIdMap.end()) {
            LOG_ERROR("Delete plugin instance id { %s } of pluginName { %s } failed,", uuid.c_str(), pluginName.c_str());
            return CRANE_FAIL;
        }

        itr->second.remove(uuid);

        if (itr->second.empty()) { _pluginInstanceIdMap.erase(itr); }

        return CRANE_SUCC;
    }

    list<string> PluginInterfaceInfo::getPluginInstanceIds(const string& pluginName) const {
        PluginInstanceIdMap::const_iterator itr = _pluginInstanceIdMap.find(pluginName);
        if (itr == _pluginInstanceIdMap.cend()) {
            LOG_ERROR("Itf name { %s } does not contain plugin instance with plugin name { %s }", _type.c_str(), pluginName.c_str());
            return list<string>();
        }

        // TODO: this is copy constructor, there is performance problem.
        return itr->second;
    }
    // #endif

}
