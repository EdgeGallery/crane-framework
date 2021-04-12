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
                    "testuser",
                    "LGPL")
