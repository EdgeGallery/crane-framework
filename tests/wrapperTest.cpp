/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 17:17:25
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-02-23 18:10:48
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

#include "gtest/gtest.h"

#include "crane.h"

#include "playerInterface.h"

using namespace std;
using namespace NS_CRANE;

TEST(Wrapper, basic) {
    cout << "Enter Wrapper basic()" << endl;
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();

    pPluginFrame->init(1, nullptr, CRANE_CRN);

    Wrapper<Itf_Player> player_mp3 = Wrapper<Itf_Player>("Itf_Player", "PlayerImplMP3", "my first plugin instance");
    player_mp3->play("play mp3...");

    Wrapper<Itf_Player> player_cd = Wrapper<Itf_Player>("Itf_Player", "PlayerImplCD", "my first plugin instance");
    player_cd->play("play cd...");

}