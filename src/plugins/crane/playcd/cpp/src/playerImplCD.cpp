/*
 *    Copyright 2020 Huawei Technologies Co., Ltd.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
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
