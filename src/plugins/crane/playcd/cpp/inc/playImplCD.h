/*
 * @Descripttion: 插件接口实现类定义
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-04 20:57:07
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-27 18:21:07
 */ 
#ifndef __PLAYIMPLCD_H__
#define __PLAYIMPLCD_H__

#include <iostream>
#include "cranePlugin.h"
#include "playerInterface.h"

class PlayerImplCD : public Itf_Player {
    public:
        unsigned int init() override;
        unsigned int play(std::string) override;
        unsigned int pause() override;
        unsigned int stop() override;

    public:
        ~PlayerImplCD();
};

#endif