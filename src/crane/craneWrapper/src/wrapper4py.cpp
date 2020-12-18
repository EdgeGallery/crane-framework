/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-29 14:29:28
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-29 14:35:36
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

PluginBaseInterface* create(string itfName, string pluginName, string description) {
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    return pPluginFrame->create(itfName, pluginName, description);
}

void destory(PluginBaseInterface* cranePlugin) {
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