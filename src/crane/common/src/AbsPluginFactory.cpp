/*
 * @Descripttion: 插件工厂的抽象类
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-09 16:05:28
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-06-17 11:45:07
 */ 

#include "AbsPluginFactory.h"

namespace NS_CRANE {
    
    const string& AbstractPluginFactory::getVersion() const {
        return version;
    }

    const string& AbstractPluginFactory::getInterfaceType() const {
        return interfaceType;
    }

    const string& AbstractPluginFactory::getPluginName() const {
        return pluginName;
    }
    
}