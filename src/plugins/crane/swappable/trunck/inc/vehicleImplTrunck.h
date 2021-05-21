/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: hwdy
 * @Date: 2021-03-08 14:57:01
 * @LastEditors: hwdy
 * @LastEditTime: 2021-03-08 20:17:29
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

#ifndef __PLUGIN_VEHICLE_IMPL_TRUNCK_H__
#define __PLUGIN_VEHICLE_IMPL_TRUNCK_H__

#include "vehicle.h"

class Trunck : public Itf_Vechile {
public:
    unsigned init() override;
    unsigned start() override;
    unsigned stop() noexcept override;

    unsigned swap_up(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) override;
    unsigned swap_down(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) override;

    const string vechileType() const override;
    void licensePlateNum(const string&)  override;
    const string licensePlateNum() const override;
    long long mileage() const override;
    void mileage(const long long m) override;
    void run() override;

public:
    ~Trunck();
private:
    static const string     _VECHILE_TYPE;
};

#endif