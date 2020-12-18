/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-27 17:30:45
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-27 18:21:14
 */
#ifndef __PLAYIMPLMP3_H__
#define __PLAYIMPLMP3_H__

#include <iostream>
#include "cranePlugin.h"
#include "playerInterface.h"

class PlayerImplMP3 : public Itf_Player {
    public:
        unsigned int init() override;
        unsigned int play(std::string) override;
        unsigned int pause() override;
        unsigned int stop() override;

    public:
        ~PlayerImplMP3();
};

#endif