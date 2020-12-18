/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-15 19:37:04
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-06-15 19:37:14
 */ 
#ifndef CRANE_ABSREGISTRY_H
#define CRANE_ABSREGISTRY_H

//#define __x86_64__

#include <string>
#include <map>
#include "AbsLibrary.h"
#include "AbsPluginFactory.h"
#include "AbsInterfaceInfo.h"
#include "PluginInterfaceInfo.h"

namespace crane {

class AbsRegistry {
    public:
        //////////////////////////////////////////////////
        /// \brief 将插件接口注册到插件系统中
        //////////////////////////////////////////////////
        virtual unsigned int registry(AbstractInterfaceInfo&) = 0;

        //////////////////////////////////////////////////////
        /// \brief 将插件动态库的插件工厂和插件实现加载到插件系统中
        //////////////////////////////////////////////////////
        virtual unsigned int load(AbsLibrary&) = 0;

        //////////////////////////////////////////////////////
        /// \brief 扫描路径下的插件so文件
        //////////////////////////////////////////////////////
        virtual unsigned int scan() = 0;

    protected:
        using PluginInterfaceMap = std::map<std::string, PluginInterfaceInfo>;
        PluginInterfaceMap _pluginItfMap;

        using PluginImplementMap = std::map<std::string,  AbsLibrary>;
        PluginImplementMap pluginMap;
        
};

}
#endif