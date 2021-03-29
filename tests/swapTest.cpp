/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-03-07 15:52:10
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-09 16:44:51
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

#include "vehicle.h"
#include "playerInterface.h"

using namespace std;
using namespace NS_CRANE;
#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)

string s_oldfile, s_newfile;
DIR* plugins_dir;

class swapTest : public testing::Test {
    protected:

        static void SetUpTestCase()
        {
            cout << "SetUpTestCase" << endl;
            char* cwd = NULL;
            // Get current work dir.
            if ((cwd = getcwd(NULL, 0)) == NULL) {
                cout << "Get current path failed." << endl;
                free(cwd);
                return;
            } else {
                cout << "Current path: " << cwd << endl;
            }

            string s_cwd(cwd);
            s_oldfile = cwd; s_newfile = cwd;
            _s_cwd = s_cwd;
            free(cwd);
            
            // Generate original plugin absulote file name.
            s_oldfile += string("/../lib/plugins/libcraneplugintrunck.so");
            // Generate new plugin absulote file name.
            s_newfile += string("/plugins/libcraneplugintrunck.so");
            cout << "s_oldfile: " << s_oldfile << endl;
            cout << "s_newfile: " << s_newfile << endl;

            // Create plugins dir below the current path.
            if (access("plugins", F_OK|R_OK|W_OK|X_OK)) {
                int ret = mkdir("plugins", MODE);
                if (ret != 0) {
                    cout << "mkdir Error: " << strerror(errno) << endl;
                    return;
                }
                plugins_dir = opendir("plugins");
            }

            // Check whether new plugin file is exist.
            if (access("plugins/libcraneplugintrunck.so", F_OK)) {
                int ret = rename(s_oldfile.c_str(), s_newfile.c_str());
                if (ret != 0) {
                    cout << "rename Error: " << strerror(errno) << endl;
                    return;
                }
            }

            // pluginFrame = AbstractPluginFrame::getPluginFrame();
            // int argc = 1; char **argv = nullptr;
            // pluginFrame->init(argc, argv, CRANE_CRN);
        }

        static void TearDownTestCase()
        {
            cout << "TearDownTestCase" << endl;
            if (!access("plugins/libcraneplugintrunck.so", F_OK)) {
                if (rename(s_newfile.c_str(), s_oldfile.c_str())) {
                    cout << "rename Error: " << strerror(errno) << endl;
                    return;
                }
            }
            if (!access("plugins", F_OK)) {
                closedir(plugins_dir);
                int ret = rmdir("plugins");
                if (ret != 0) {
                    cout << "rmdir Error: " << strerror(errno) << endl;
                }
            }
        }

        virtual void SetUp()
        {
            cout << endl << endl;
            cout << "SetUp()" << endl;
        }

        virtual void TearDown()
        {
            cout << "TearDown()" << endl;
            cout << endl << endl;
        }

        static string _s_cwd;
};

string swapTest::_s_cwd = "";

TEST_F(swapTest, swap_plugin_by_file) {
    cout << endl << endl;
    cout << "Enter swap_plugin_by_file()" << endl;
    cout << endl << endl;

    AbstractPluginFrame* pluginFrame;
    pluginFrame = AbstractPluginFrame::getPluginFrame();
    int argc = 1; char **argv = nullptr;
    pluginFrame->init(argc, argv, CRANE_CRN);

    string id{};
    shared_ptr<Wrapper> wrapper_vechile = pluginFrame->createSwappablePlugin("Itf_Vechile", "Car", id, "");

    cout << "Swappable plugin id: " << id << endl;
    cout << "Vechile Type: " << plugin_cast<Itf_Vechile>(wrapper_vechile)->vechileType() << endl;
    plugin_cast<Itf_Vechile>(wrapper_vechile)->start();

    this_thread::sleep_for(chrono::seconds(1));
    plugin_cast<Itf_Vechile>(wrapper_vechile)->licensePlateNum("粤A 672KR");
    cout << " wrapper_player_cd use_count: " << wrapper_vechile->p().use_count() << endl;
    cout << ":::::::->" <<wrapper_vechile->p().get() << endl;
    void* stale_p = wrapper_vechile->p().get();

    this_thread::sleep_for(chrono::seconds(1));

    cout << "&&&&&&&&&&&" << "Swapping plugin from car to trunck now..." << "&&&&&&&&&&&" << endl;
    string fresh_plugin_filename = _s_cwd + string("/plugins/libcraneplugintrunck.so");
    auto ret = pluginFrame->swapByFilename(id, fresh_plugin_filename);
    if (ret == CRANE_SUCC) {
        cout << "&&&&&&&&&&&" << "Swapping plugin from car to trunck successfully..." << "&&&&&&&&&&&" << endl;
    } else {
        cout << "&&&&&&&&&&&" << "Swapping plugin from car to trunck unsuccessfully..." << "&&&&&&&&&&&" << endl;
    }
    ASSERT_EQ(CRANE_SUCC, ret);

    this_thread::sleep_for(chrono::seconds(1));

    cout << "Vechile Type: " << plugin_cast<Itf_Vechile>(wrapper_vechile)->vechileType() << endl;
    plugin_cast<Itf_Vechile>(wrapper_vechile)->licensePlateNum("鄂B 672KR");
    this_thread::sleep_for(chrono::seconds(1));
    plugin_cast<Itf_Vechile>(wrapper_vechile)->stop();
    cout << ":::::::->" <<wrapper_vechile->p().get() << endl;
    void* fresh_p = wrapper_vechile->p().get();
    ASSERT_NE(stale_p, fresh_p);
}

TEST_F(swapTest, swap_plugin_by_id) {
    cout << endl << endl;
    cout << "Enter swap_plugin_by_id()" << endl;
    cout << endl << endl;

    AbstractPluginFrame* pluginFrame;
    pluginFrame = AbstractPluginFrame::getPluginFrame();
    int argc = 1; char **argv = nullptr;
    pluginFrame->init(argc, argv, CRANE_CRN);

    string swappable_plugin_id {};
    shared_ptr<Wrapper> wrapper_vechile = pluginFrame->createSwappablePlugin("Itf_Vechile", "Car", swappable_plugin_id, "");
    cout << "Swappable plugin id: " << swappable_plugin_id << endl;
    plugin_cast<Itf_Vechile>(wrapper_vechile)->start();
    this_thread::sleep_for(chrono::seconds(1));

    plugin_cast<Itf_Vechile>(wrapper_vechile)->licensePlateNum("粤A 672KR");
    cout << " wrapper_player_cd use_count: " << wrapper_vechile->p().use_count() << endl;
    cout << ":::::::->" <<wrapper_vechile->p().get() << endl;
    void* stale_p = wrapper_vechile->p().get();

    string fresh_plugin_id {};
    string fresh_plugin_filename = _s_cwd + string("/plugins/libcraneplugintrunck.so");
    cout << "fresh_plugin_filename: " << fresh_plugin_filename << endl;
    pluginFrame->load(fresh_plugin_filename);
    pluginFrame->create("Itf_Vechile", "Trunck", fresh_plugin_id, "");
    cout << "Fresh plugin id: " << fresh_plugin_id << endl;

    cout << "&&&&&&&&&&&" << "Swapping plugin from car to trunck now..." << "&&&&&&&&&&&" << endl;
    unsigned ret = pluginFrame->swapById(swappable_plugin_id, fresh_plugin_id);
    if (ret == CRANE_SUCC) {
        cout << "&&&&&&&&&&&" << "Swapping plugin from car to trunck successfully..." << "&&&&&&&&&&&" << endl;
    } else {
        cout << "&&&&&&&&&&&" << "Swapping plugin from car to trunck unsuccessfully..." << "&&&&&&&&&&&" << endl;
    }
    ASSERT_EQ(CRANE_SUCC, ret);

    this_thread::sleep_for(chrono::seconds(1));

    cout << "Vechile Type: " << plugin_cast<Itf_Vechile>(wrapper_vechile)->vechileType() << endl;
    plugin_cast<Itf_Vechile>(wrapper_vechile)->licensePlateNum("鄂B 672KR");
    this_thread::sleep_for(chrono::seconds(1));
    plugin_cast<Itf_Vechile>(wrapper_vechile)->stop();
    cout << ":::::::->" <<wrapper_vechile->p().get() << endl;
    void* fresh_p = wrapper_vechile->p().get();
    ASSERT_NE(stale_p, fresh_p);
}