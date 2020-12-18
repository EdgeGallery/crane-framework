/*
 * @Descripttion: Crane插件系统的插件基类接口
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-10 12:26:00
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-06-20 15:56:21
 */ 
#ifndef __CRANE_PLUGINBASEINTERFACE_H__
#define __CRANE_PLUGINBASEINTERFACE_H__

//#define __x86_64__

#include <string>

#include "craneMacro.h"

namespace NS_CRANE {

class PluginBaseInterface {
    public:
        virtual unsigned int init() = 0;

    public:
        virtual ~PluginBaseInterface() {};
    
};

}

#endif