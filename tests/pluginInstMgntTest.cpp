/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-03-04 19:31:52
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-04 21:31:34
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

TEST(PluginInstMgnt, create_plugin_instance) {
    cout << endl << endl;
    cout <<"Enter Wrapper create_plugin_instance()" << endl;
    AbstractPluginFrame* pluginFrame = AbstractPluginFrame::getPluginFrame();
    pluginFrame->init(1, nullptr, CRANE_CRN);

    //*** Create cd and mp3 player.
    string player_cd_id_1;
    shared_ptr<Itf_Player> player_cd_1 = dynamic_pointer_cast<Itf_Player>(pluginFrame->create("Itf_Player", "PlayerImplCD", player_cd_id_1, ""));
    player_cd_1->play("player_cd_1 play cd...");

    string player_mp3_id_1;
    shared_ptr<Itf_Player> player_mp3_1 = dynamic_pointer_cast<Itf_Player>(pluginFrame->create("Itf_Player", "PlayerImplMP3", player_mp3_id_1, ""));
    player_mp3_1->play("player_cd_1 play cd...");

    //*** Fetch the first instance of PlayerImplCD instance.
    shared_ptr<Itf_Player> duplicate_player_cd_1 = dynamic_pointer_cast<Itf_Player>(pluginFrame->instance("Itf_Player", "PlayerImplCD"));
    ASSERT_EQ(player_cd_1.get(), duplicate_player_cd_1.get());
    ASSERT_STREQ(player_cd_id_1.c_str(), pluginFrame->id("Itf_Player", "PlayerImplCD").c_str());
    duplicate_player_cd_1->play("duplicate_play_cd_1 play cd...");

    //*** Fetch the first instance of PlayerImplMP3 instance.
    shared_ptr<Itf_Player> duplicate_player_mp3_1 = dynamic_pointer_cast<Itf_Player>(pluginFrame->instance("Itf_Player", "PlayerImplMP3"));
    ASSERT_EQ(player_mp3_1.get(), duplicate_player_mp3_1.get());
    ASSERT_STREQ(player_mp3_id_1.c_str(), pluginFrame->id("Itf_Player", "PlayerImplMP3").c_str());
    duplicate_player_mp3_1->play("duplicate_play_cd_1 play cd...");

    //*** Add another cd and mp3 player.
    string player_cd_id_2 = "player_cd_id_2";
    shared_ptr<Itf_Player> player_cd_2 = dynamic_pointer_cast<Itf_Player>(pluginFrame->create("Itf_Player", "PlayerImplCD", player_cd_id_1, ""));
    player_cd_1->play("player_cd_2 play cd...");

    string player_mp3_id_2 = "player_mp3_id_2 ";
    shared_ptr<Itf_Player> player_mp3_2 = dynamic_pointer_cast<Itf_Player>(pluginFrame->create("Itf_Player", "PlayerImplMP3", player_mp3_id_1, ""));
    player_mp3_1->play("player_cd_2 play cd...");

    //*** Fetch instance of PlayerImplCD instance through id.
    shared_ptr<Itf_Player> duplicate_player_cd_2 = dynamic_pointer_cast<Itf_Player>(pluginFrame->instance(player_cd_id_2));
    ASSERT_EQ(player_cd_2.get(), duplicate_player_cd_2.get());

    shared_ptr<Itf_Player> duplicate_player_mp3_2 = dynamic_pointer_cast<Itf_Player>(pluginFrame->instance(player_mp3_id_2));
    ASSERT_EQ(player_mp3_2.get(), duplicate_player_mp3_2.get());

}