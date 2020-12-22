/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-10 14:29:21
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-14 16:29:40
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

CRANE_PLUGIN_DEFINE(
    "1.0.0",        //Version of the plugin interface
    Itf_Player,     //Name of the plugin interface 
    "1.0.1",        //Required the version of the plugin framework.    
    PlayerImplCD,   //Name of the plugin interface implemention.
    "CD player which is implement the Itf_Player interface.",   //Description of the plugin implemention
    "dongyin@huawei.com",       //Author of the plugin implemention.
    "LGPL"                      //License.
    )