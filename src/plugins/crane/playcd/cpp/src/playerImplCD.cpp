/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-10 14:29:21
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-29 11:44:50
 */ 
#include "playImplCD.h"

unsigned int PlayerImplCD::init() {
    std::cout<<"Crane plugin: CD player is initialized."<<endl;
    return 0;
}

unsigned int PlayerImplCD::play(std::string str) {
    std::cout<<"Crane plugin: CD player is playing with audio stream:{ "<<str<<" }"<<endl;
    return 0;
}

unsigned int PlayerImplCD::pause() {
    std::cout<<"Crane plugin: CD player is paused."<<endl;
    return 0;
}

unsigned int PlayerImplCD::stop() {
    std::cout<<"Crane plugin: CD player has stopped."<<endl;
    return 0;
}

PlayerImplCD::~PlayerImplCD() {
    std::cout<<"Crane plugin: CD player release resource."<<endl;
}

CRANE_PLUGIN_DEFINE("1.0.0",    //插件接口版本
                    Itf_Player,
                    "1.0.1",    //要求的插件框架版本   
                    PlayerImplCD,
                    "CD player which is implement the Itf_Player interface.",
                    "dongyin@huawei.com",
                    "LGPL")