/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 16:39:20
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

#ifndef __PLUGIN_PLAYERINTERFACE_H__
#define __PLUGIN_PLAYERINTERFACE_H__

#include "PluginBaseInterface.h"

class Itf_Player : public NS_CRANE::PluginBaseInterface {
    public:
        virtual unsigned int play(std::string) = 0;
        virtual unsigned int pause() = 0;
};

#endif
