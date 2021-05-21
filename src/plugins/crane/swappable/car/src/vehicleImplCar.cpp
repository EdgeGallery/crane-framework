/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: hwdy
 * @Date: 2021-03-08 14:19:40
 * @LastEditors: hwdy
 * @LastEditTime: 2021-03-27 08:56:44
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

#include "vehicleImplCar.h"
const string Car::_VECHILE_TYPE{"CAR"};

unsigned Car::init() {
    cout << "Crane plugin: Car is initialized." << endl;
    licensePlateNum_ = "粤B 672KR";
    return CRANE_SUCC;
}

unsigned Car::start() {
    cout << "Crane plugin: Car plugin is start..." << endl;
    isRunning_.store(true);
    t_ = thread(&Car::run, this);
    return CRANE_SUCC;
}

unsigned Car::stop() noexcept {
    cout << "Crane plugin: Car plugin is stop..." << endl;
    isRunning_.store(false);
    if (t_.joinable()) { t_.join(); }
    return CRANE_SUCC;
}

unsigned Car::swap_down(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) {
    cout << "Crane plugin: Car is swapping down." << endl;
    touch(stale, fresh);
    return CRANE_SUCC;
}

unsigned Car::swap_up(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) {
    cout << "Crane plugin: Car is swapping up." << endl;
    touch(stale, fresh);
    return CRANE_SUCC;
}

const string Car::vechileType() const {
    return Car::_VECHILE_TYPE;
}

void Car::licensePlateNum(const string& num) {
    licensePlateNum_ = num;
}

const string Car::licensePlateNum() const {
    return licensePlateNum_;
}

long long Car::mileage() const {
    return mileage_;
}

void Car::mileage(const long long m) {
    mileage_ = m;
}

void Car::run() {
    cout << "Crane plugin: Car{ " << licensePlateNum_ << " } is running now..." << endl;
    while(isRunning_.load()) {
        this_thread::sleep_for(chrono::milliseconds(10));
        ++mileage_;
        if ((mileage_ % 10) == 0) {
            cout << _VECHILE_TYPE <<" " << licensePlateNum_ << " has run " << mileage_ << " Kilometers." << endl;
        }
    }
    return;
}

Car::~Car() {
    cout << "Crane plugin: Car release resource." << endl;
}

CRANE_PLUGIN_DEFINE("1.0.0",
                    Itf_Vechile,
                    "1.0.1",
                    Car,
                    "Car which is implement the Itf_Vechile interface.",
                    "hwdy",
                    "LGPL")