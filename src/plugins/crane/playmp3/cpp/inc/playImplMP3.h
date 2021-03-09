/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 17:34:43
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

#ifndef __PLAYIMPLMP3_H__
#define __PLAYIMPLMP3_H__

#include <iostream>
#include "cranePlugin.h"
#include "playerInterface.h"

class PlayerImplMP3 : public Itf_Player {
    public:
        unsigned int init() override;
        unsigned start() override;
        unsigned stop() noexcept override;

        unsigned int play(std::string) override;
        unsigned int pause() override;

    public:
        ~PlayerImplMP3();
};

#endif
