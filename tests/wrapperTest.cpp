/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 15:52:48
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-04 21:23:43
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

TEST(Wrapper, create_plugin_without_hold_instance) {
    cout << endl << endl;
    cout << "Enter Wrapper create_plugin_without_hold_instance()" << endl;
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();

    pPluginFrame->init(1, nullptr, CRANE_CRN);

    Wrapper<Itf_Player> player_mp3 = Wrapper<Itf_Player>("Itf_Player", "PlayerImplMP3", "my first plugin instance");
    ASSERT_TRUE(player_mp3);
    player_mp3->play("play mp3...");

    Wrapper<Itf_Player> player_cd1 = Wrapper<Itf_Player>("Itf_Player", "PlayerImplCD", "my first plugin instance");
    ASSERT_TRUE(player_cd1);
    player_cd1->play("play cd...");
}

TEST(Wrapper, create_plugin_with_hold_instance) {
    cout << endl << endl;
    cout << "Enter Wrapper create_plugin_with_hold_instance()" << endl;
    AbstractPluginFrame* pluginFrame = AbstractPluginFrame::getPluginFrame();
    pluginFrame->init(1, nullptr, CRANE_CRN);

    string id_1;
    Wrapper<Itf_Player> wrapper_player_cd_1 = Wrapper<Itf_Player>("Itf_Player", "PlayerImplCD", id_1, "my first plugin instance");
    ASSERT_TRUE(wrapper_player_cd_1);
    wrapper_player_cd_1->play("play_cd is playing cd...");

    string id_2 = "12345678";
    Wrapper<Itf_Player> wrapper_player_cd_2 = Wrapper<Itf_Player>("Itf_Player", "PlayerImplCD", id_2, "my first plugin instance");
    ASSERT_TRUE(wrapper_player_cd_2);
    ASSERT_EQ("12345678", id_2);
    wrapper_player_cd_2->play("play_cd is playing cd...");
}

TEST(Wrapper, duplicate_plugin_from_framework) {
    cout << endl << endl;
    cout << "Enter Wrapper duplicate_plugin_from_framework()" << endl;
    AbstractPluginFrame* pluginFrame = AbstractPluginFrame::getPluginFrame();
    pluginFrame->init(1, nullptr, CRANE_CRN);

    string id = "";
    Wrapper<Itf_Player> wrapper_player_cd = Wrapper<Itf_Player>("Itf_Player", "PlayerImplCD", id, "my first plugin instance");
    ASSERT_TRUE(wrapper_player_cd);
    ASSERT_TRUE(!id.empty());
    cout<<"plugin id: "<<id<<endl;
    wrapper_player_cd->play("play_cd is playing cd...");

    shared_ptr<Itf_Player> play_cd_duplicated_1 = dynamic_pointer_cast<Itf_Player>(pluginFrame->instance(id));
    ASSERT_EQ(wrapper_player_cd.p().get(), play_cd_duplicated_1.get());
    play_cd_duplicated_1->play("play_cd_duplicated_1 is playing cd...");
    
    cout<<"plugin id: "<<pluginFrame->id("Itf_Player", "PlayerImplCD")<<endl;
    // Fetch the FIRST instance through itf type name and plugin name.
    shared_ptr<Itf_Player> play_cd_duplicated_2 = dynamic_pointer_cast<Itf_Player>(pluginFrame->instance("Itf_Player", "PlayerImplCD"));
    // So it is not equel to wrapper_player_cd.
    // ASSERT_EQ(wrapper_player_cd.p().get(), play_cd_duplicated_2.get());
    play_cd_duplicated_2->play("play_cd_duplicated_2 is playing cd...");

    Wrapper<Itf_Player> wrapper_play_cd_duplicated(dynamic_pointer_cast<Itf_Player>(pluginFrame->instance(id)));
    wrapper_play_cd_duplicated->play("wrapper_play_cd_duplicated is playing cd...");
}