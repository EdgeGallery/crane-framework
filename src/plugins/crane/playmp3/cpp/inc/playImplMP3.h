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
        unsigned int play(std::string) override;
        unsigned int pause() override;
        unsigned int stop() override;

    public:
        ~PlayerImplMP3();
};

#endif
