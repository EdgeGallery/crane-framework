/*
 * @Descripttion: Wrap the plugin instance. So when client try to call plugin interface, 
 *                  Wrapper class could add some behavior before or after the invoking 
 *                  plugin's interface.
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 11:11:40
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-04 19:21:50
 */

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

#ifndef __WRAPPER_H__
#define __WRAPPER_H__

#include "common.h"

using namespace std;

namespace NS_CRANE {
template <typename PT>
class Wrapper final {
public:
    Wrapper() = default;

    /**
     * @Descripttion: Construct a plugin wrapper instance but does not store the plugin instance into framework.
     * @Param: type: plugin interface name
     * @Param: pluginName: plugin class name
     * @Param: description: description information of this plugin instance. 
     */         
    Wrapper(const string& type, const string& pluginName, const string& description) {
        AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
        _plugin.reset(dynamic_cast<PT*>(pPluginFrame->create(type, pluginName, description)));
        _id = NULL_PLUGIN_ID;
    }
    // #if 0 // dongyin 2-27

    /**
     * @Descripttion: Construct a plugin wrapper instance and store the plugin instance into framework.
     * @Param: type: plugin interface name
     * @Param: pluginName: plugin class name
     * @Param: id: plugin instance id which could be used to get plugin instance from framework later. 
     * @Param: description: description information of this plugin instance. 
     */         
    Wrapper(const string& type, const string& pluginName, string& id, const string& description) {
        AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
        _plugin = dynamic_pointer_cast<PT>(pPluginFrame->create(type, pluginName, id, description));
        _id = id;
    }

    /**
     * @Descripttion: Construct a plugin wrapper instance through the shared_ptr<PT> instance of the plugin.
     * @Param: p: shared_ptr<PT> instance of the plugin.
     */         
    Wrapper(shared_ptr<PT> p) {
        _plugin = p;
    }
    // #endif 
    Wrapper(const Wrapper& w) = default;
    Wrapper& operator=(const Wrapper& w) = default;
    
    Wrapper(Wrapper&& w) = default;
    Wrapper& operator=(Wrapper&& w) = default;

    ~Wrapper() = default;

    bool operator==(const Wrapper& w) const {
        return this->_plugin == w._plugin;
    }

    bool operator!=(const Wrapper& w) const {
        return this->_plugin != w._plugin;
    }

    operator bool() const {
        return _plugin != nullptr;
    }
    
    shared_ptr<PT> operator->() const {
        return _plugin;
    }

    shared_ptr<PT> p() const {
        return _plugin;
    }

    const string& id() const {
        return _id;
    }

    bool isNullId() const {
        return _id.compare(NULL_PLUGIN_ID) == 0 ? true : false;
    }
    
private:
    shared_ptr<PT>              _plugin;
    string                      _id = NULL_PLUGIN_ID;
};

}

#endif