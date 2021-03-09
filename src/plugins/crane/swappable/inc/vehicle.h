/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-03-08 14:07:38
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 20:39:29
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
#ifndef __PLUGIN_VEHICLE_H__
#define __PLUGIN_VEHICLE_H__

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

#include "cranePlugin.h"

using namespace std;
using namespace NS_CRANE;

class Itf_Vechile : public PluginBaseInterface, public PluginSwappable {
public:
    virtual const string vechileType() const = 0;
    virtual void licensePlateNum(const string&) = 0;
    virtual const string licensePlateNum() const = 0;
    virtual long long mileage() const = 0;
    virtual void mileage(const long long m) = 0;
    virtual void run() = 0;
    
protected:
    string          licensePlateNum_;
    long long       mileage_ = 0;
    atomic<bool>    isRunning_{false};
    thread          t_;
};

template<typename ... Args> inline void touch(Args&& ...) {}

#endif