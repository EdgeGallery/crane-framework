/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: hwdy
 * @Date: 2021-02-23 15:52:48
 * @LastEditors: hwdy
 * @LastEditTime: 2021-03-31 10:34:02
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

#if 0
TEST(Wrapper, create_plugin_without_hold_instance) {
#ifdef BUILD_WRAPPER_CONSTRUCTOR
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
#endif
}

TEST(Wrapper, create_plugin_with_hold_instance) {
#ifdef BUILD_WRAPPER_CONSTRUCTOR
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
#endif
}

TEST(Wrapper, duplicate_plugin_from_framework) {
#ifdef BUILD_WRAPPER_CONSTRUCTOR
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
#endif
}
#endif

class WrapperTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        pluginFrame = AbstractPluginFrame::getPluginFrame();
        pluginFrame->init(1, nullptr, CRANE_CRN);
    }

    virtual void TearDown() {
        AbstractPluginFrame::relPluginFrame();
    }
    
    AbstractPluginFrame* pluginFrame = nullptr;
};

TEST_F(WrapperTest, create_wrapper_plugin_through_type_and_plugin_name) {
    cout << endl << endl;
    cout << "Enter WrapperTest create_wrapper_plugin_through_type_and_plugin_name()" << endl;

    //*** Create wrapper plugin through the interface type and plugin implement class name.
    string id{};
    shared_ptr<Wrapper> wrapper_player_cd = 
        pluginFrame->createSwappablePlugin("Itf_Player", "PlayerImplCD", id, "");
    cout << "Swappable plugin id: " << id << endl;
    
    plugin_cast<Itf_Player>(wrapper_player_cd)->play("wrapper_player_cd is playing cd...");

    //*** Check reference count of shared_ptr<plugin>, which is refered by 
    //    _pluginInstanceMap, _pluginSwappableInstanceMap and wrapper_player_cd.
    cout << " wrapper_player_cd use_count: " << wrapper_player_cd->p().use_count() << endl;
    ASSERT_EQ(3, plugin_cast<Itf_Player>(wrapper_player_cd).use_count());

    //*** Fetch wrapper plugin from the registry through wrapper plugin id.
    shared_ptr<Wrapper> duplicated_wrapper_player_cd = 
        pluginFrame->fetchSwappablePlugin(id);
    plugin_cast<Itf_Player>(duplicated_wrapper_player_cd)->play("wrapper_player_cd is playing cd...");

    //*** Check reference count of shared_ptr<plugin>, which is refered by 
    //    _pluginInstanceMap, _pluginSwappableInstanceMap and duplicated_wrapper_player_cd.
    cout << "duplicated_wrapper_player_cd use_count: " << duplicated_wrapper_player_cd->p().use_count() << endl;
    ASSERT_EQ(3, plugin_cast<Itf_Player>(duplicated_wrapper_player_cd).use_count());

    ASSERT_EQ(wrapper_player_cd->p().get(), duplicated_wrapper_player_cd->p().get());
    cout << ":::::::->" <<wrapper_player_cd->p().get() << endl;
    cout << ":::::::->" <<duplicated_wrapper_player_cd->p().get() << endl;

    //*** Release swapperable plugin instance.
    pluginFrame->releaseSwappablePlugin(std::move(wrapper_player_cd));
    pluginFrame->releaseSwappablePlugin(std::move(duplicated_wrapper_player_cd));

    //*** Unload the library of the plugin from the process memory space.
    unsigned ret = CRANE_FAIL;
    ret = pluginFrame->unload("Itf_Player", "PlayerImplCD");
    ASSERT_EQ(CRANE_SUCC, ret);
}