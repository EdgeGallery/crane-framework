/*
 * @Descripttion: Wrap the plugin instance. So when client try to call plugin interface, 
 *                  Wrapper class could add some behavior before or after the invoking 
 *                  plugin's interface.
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 11:11:40
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-02-23 18:01:58
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
    Wrapper(const string& type, const string& pluginName, const string& description) {
        AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
        //_plugin(make_shared<PT>(dynamic_cast<PT*>(pPluginFrame->create(type, pluginName, description))));
        _plugin.reset(
         dynamic_cast<PT*>(pPluginFrame->create(type, pluginName, description)));
    }
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
    
private:
    shared_ptr<PT>     _plugin;
};

}

#endif