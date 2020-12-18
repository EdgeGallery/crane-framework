/*
 * @Descripttion: 插件接口定义
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-05 09:57:34
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-06-19 17:21:13
 */ 
#ifndef __PLUGIN_PLAYERINTERFACE_H__
#define __PLUGIN_PLAYERINTERFACE_H__

#include "PluginBaseInterface.h"

class Itf_Player : public NS_CRANE::PluginBaseInterface {
    public:
        virtual unsigned int play(std::string) = 0;
        virtual unsigned int pause() = 0;
        virtual unsigned int stop() = 0;
};

#endif