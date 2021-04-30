/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 17:28:47
 */
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


#include "playImplMP3.h"

unsigned int PlayerImplMP3::init() {
    std::cout<<"Crane plugin: MP3 player is initialized."<<std::endl;
    return CRANE_SUCC;
}

unsigned PlayerImplMP3::start() {
    cout << "Crane plugin: PlayerImplMP3 plugin is start..." << endl;
    return CRANE_SUCC;
}

unsigned PlayerImplMP3::stop() noexcept {
    cout << "Crane plugin: PlayerImplMP3 plugin is stop..." << endl;
    return CRANE_SUCC;
}

unsigned int PlayerImplMP3::play(std::string str) {
    std::cout<<"Crane plugin: MP3 player is playing with audio stream:{ "<<str<<" }"<<std::endl;
    return 0;
}

unsigned int PlayerImplMP3::pause() {
    std::cout<<"Crane plugin: MP3 player is paused."<<std::endl;
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
                    "testuser",
                    "LGPL")
