#include "singletonImpl.h"

unsigned SingletonImpl::init() {
    std::cout<<"SingletonImpl::init()"<<std::endl;
    return 0;
}

unsigned SingletonImpl::setName(std::string& name) {
    _name = name;
    return 0;
}

unsigned SingletonImpl::showName() {
    std::cout<<"Name: { "<<_name<<" } "<<std::endl;
    return 0;
}

CRANE_PLUGIN_DEFINE_SINGLETON("1.0.0",    //插件接口版本
                    Itf_Singleton,
                    "1.0.1",    //要求的插件框架版本   
                    SingletonImpl,
                    "Test crane singleton plugin.",
                    "dongyin@huawei.com",
                    "LGPL")