/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-30 09:33:37
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

#include <iostream>

extern "C" {
    #include <gst/gst.h>
}

#include "PluginBaseInterface.h"
#include "AbsPluginFrame.h"
#include "wrapper4py.h"

using namespace NS_CRANE;


unsigned init(unsigned init_type) {
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();

    int arg = 0; char** argv = nullptr;
    unsigned ret = pPluginFrame->init(arg, argv, init_type);

    return ret;
}

PluginBase* create(string itfName, string pluginName, string id) {
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    return pPluginFrame->create(itfName, pluginName, id);
}

void destory(PluginBase* cranePlugin) {
    if (cranePlugin == nullptr) { return; }

    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    pPluginFrame->destory(cranePlugin);
}

void* create(string factoryName, string pluginName) {
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    return (GstElement*)pPluginFrame->create(factoryName, pluginName);
}

void destory(void * plugin) {
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    pPluginFrame->destory(plugin);
}

unsigned load(const std::string& filename) {
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    return pPluginFrame->load(filename);
}

void unload(const std::string& type, const std::string& pluginName) {
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    pPluginFrame->unload(type, pluginName);
}
