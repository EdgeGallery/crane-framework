/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-22 17:08:46
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-23 10:47:56
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