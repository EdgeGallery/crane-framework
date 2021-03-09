/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-03-08 14:57:19
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 20:38:36
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

#include "vehicleImplTrunck.h"
const string Trunck::_VECHILE_TYPE{"TRUNCK"};

unsigned Trunck::init() {
    cout << "Crane plugin: Trunck is initialized." << endl;
    licensePlateNum_ = "鄂A 672KR";
    return CRANE_SUCC;
}

unsigned Trunck::start() {
    cout << "Crane plugin: Trunck plugin is start..." << endl;
    isRunning_.store(true);
    t_ = thread(&Trunck::run, this);
    return CRANE_SUCC;    
}

unsigned Trunck::stop() noexcept {
    cout << "Crane plugin: Trunck plugin is stop..." << endl;
    isRunning_.store(false);
    if (t_.joinable()) { t_.join(); }
    return CRANE_SUCC;
}

unsigned Trunck::swap_up(shared_ptr<PluginBaseInterface> stale, shared_ptr<PluginBaseInterface> fresh) {
    cout << "Crane plugin: Trunck is swapping up." << endl;
    long long m = dynamic_pointer_cast<Itf_Vechile>(stale)->mileage();
    dynamic_pointer_cast<Itf_Vechile>(fresh)->mileage(m);
    return CRANE_SUCC;
}

unsigned Trunck::swap_down(shared_ptr<PluginBaseInterface> stale, shared_ptr<PluginBaseInterface> fresh) {
    cout << "Crane plugin: Trunck is swapping down." << endl;
    touch(stale, fresh);
    return CRANE_SUCC;
}

const string Trunck::vechileType() const {
    return Trunck::_VECHILE_TYPE;
}

void Trunck::licensePlateNum(const string& num) {
    licensePlateNum_ = num;
}

const string Trunck::licensePlateNum() const {
    return licensePlateNum_;
}

long long Trunck::mileage() const {
    return mileage_;
}

void Trunck::mileage(const long long m) {
    mileage_ = m;
}

void Trunck::run() {
    cout << "Crane plugin: Trunck{ " << licensePlateNum_ << " } is running now..." << endl;
    while(isRunning_.load()) {
        this_thread::sleep_for(chrono::milliseconds(10));
        ++mileage_;
        if ((mileage_ % 10) == 0) {
            cout << _VECHILE_TYPE <<" " << licensePlateNum_ << " has run " << mileage_ << " Kilometers." << endl;
        }
    }
    return;    
}

Trunck::~Trunck() {
    cout << "Crane plugin: Trunck release resource." << endl;
}

CRANE_PLUGIN_DEFINE("1.0.0",
                    Itf_Vechile,
                    "1.0.1",  
                    Trunck,
                    "Trunck which is implement the Itf_Vechile interface.",
                    "dongyin@huawei.com",
                    "LGPL")