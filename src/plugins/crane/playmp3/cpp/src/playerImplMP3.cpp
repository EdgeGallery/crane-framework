/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-27 17:31:21
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-27 20:39:20
 */

#include "playImplMP3.h"

unsigned int PlayerImplMP3::init() {
    std::cout<<"Crane plugin: MP3 player is initialized."<<std::endl;
    return 0;
}

unsigned int PlayerImplMP3::play(std::string str) {
    std::cout<<"Crane plugin: MP3 player is playing with audio stream:{ "<<str<<" }"<<std::endl;
    return 0;
}

unsigned int PlayerImplMP3::pause() {
    std::cout<<"Crane plugin: MP3 player is paused."<<std::endl;
    return 0;
}

unsigned int PlayerImplMP3::stop() {
    std::cout<<"Crane plugin: MP3 player has stopped."<<std::endl;
    return 0;
}

PlayerImplMP3::~PlayerImplMP3() {
    std::cout<<"Crane plugin: MP3 player release resource."<<std::endl;
}

CRANE_PLUGIN_DEFINE("1.0.0",    //插件接口版本
                    Itf_Player,
                    "1.0.1",    //要求的插件框架版本   
                    PlayerImplMP3,
                    "MP3 player which is implement the Itf_Player interface.",
                    "dongyin@huawei.com",
                    "LGPL")