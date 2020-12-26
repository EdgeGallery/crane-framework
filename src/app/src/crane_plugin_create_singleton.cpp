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

//Crane H files
#include "crane.h"

#include "singletonImpl.h"

using namespace NS_CRANE;

int main(int argc, char** argv) {
    //*** Create a singleton object of plugin frame.
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();

    //*** Initialize Crane and Gstreamer system.
    pPluginFrame->init(argc, argv, CRANE_CRN);

    //*** Create "SingletonImpl" instance of crane singtonImpl plugin.
    Itf_Singleton* singleton1 = dynamic_cast<Itf_Singleton*>(
        pPluginFrame->create("Itf_Singleton", "SingletonImpl", "test crane singleton plugin"));
    
    //*** Using the "PlayerImplCD" plugin instance.
    if (singleton1 == nullptr) {
        return 0;
    }
    singleton1->showName();
    string newName = string("dongkezhen");
    singleton1->setName(newName);
    singleton1->showName();
    
    //*** Create "SingletonImpl" instance of crane singtonImpl plugin again.
    Itf_Singleton* singleton2 = dynamic_cast<Itf_Singleton*>(
        pPluginFrame->create("Itf_Singleton", "SingletonImpl", "test crane singleton plugin"));

    if (singleton2 == nullptr) {
        return 0;
    }
    
    singleton2->showName();

    cout<<"singleton1: "<<singleton1<<std::endl;
    cout<<"singleton2: "<<singleton2<<std::endl;

    pPluginFrame->destory(singleton2);
    return 0;
    
}