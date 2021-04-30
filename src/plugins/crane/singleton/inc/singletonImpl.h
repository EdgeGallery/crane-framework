/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 16:35:36
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

#ifndef __SINGLETONIMPL_H__
#define __SINGLETONIMPL_H__

#include <iostream>
#include "cranePlugin.h"
#include "singletonInterface.h"

class SingletonImpl : public Itf_Singleton {
    public:
        SingletonImpl () { std::cout<<"SingletonImpl()"<<std::endl; };
        SingletonImpl (std::string name) : _name(name) { std::cout<<"SingletonImpl(string)"<<std::endl;};
        unsigned int init() override;
        unsigned start() override;
        unsigned stop() noexcept override;

        unsigned setName(std::string&) override;
        unsigned showName() override;
        ~SingletonImpl() {std::cout<<"~SingletonImpl()"<<std::endl;};

    private:
        std::string     _name = "dongyin";
};

#endif
