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

#include <stdio.h>

#include "gtest/gtest.h"

#include "crane.h"

#include "playerInterface.h"

using namespace std;
using namespace NS_CRANE;

#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)

AbstractPluginFrame* pPluginFrame;
string s_oldfile, s_newfile;
DIR* plugins_dir;


class coreTest : public testing::Test {
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
            s_oldfile += string("/../lib/plugins/libcranepluginplayermp3.so");
            // Generate new plugin absulote file name.
            s_newfile += string("/plugins/libcranepluginplayermp3.so");
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
            if (access("plugins/libcranepluginplayermp3.so", F_OK)) {
                int ret = rename(s_oldfile.c_str(), s_newfile.c_str());
                if (ret != 0) {
                    cout << "rename Error: " << strerror(errno) << endl;
                    return;
                }
            }

            pPluginFrame = AbstractPluginFrame::getPluginFrame();
            int argc = 1; char **argv = nullptr;
            pPluginFrame->init(argc, argv, CRANE_CRN);
        }

        static void TearDownTestCase()
        {
            cout << "TearDownTestCase" << endl;
            if (!access("plugins/libcranepluginplayermp3.so", F_OK)) {
                if (rename(s_newfile.c_str(), s_oldfile.c_str())) {
                    cout << "rename Error: " << strerror(errno) << endl;
                    return;
                }
            }
            if (!access("plugins", F_OK)) {
                closedir(plugins_dir);
                int ret = rmdir("plugins");
                if (!ret) {
                    cout << "rmdir Error: " << strerror(errno) << endl;
                }
            }
        }

        virtual void SetUp()
        {
            cout << "SetUp()" << endl;
        }

        virtual void TearDown()
        {
            cout << "TearDown()" << endl;
        }

        static string _s_cwd;
};
string coreTest::_s_cwd = "";

TEST_F(coreTest, load_specail_plugin)
{
    string plugin_filename = _s_cwd + string("/plugins/libcranepluginplayermp3.so");
    pPluginFrame->load(plugin_filename);

    string id1 {"my first plugin instance of mp3"};
    Itf_Player* playerMP3 = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplMP3", id1));
    if (playerMP3) {
        playerMP3->play("play mp3...");
    }

    string id2 {"my first plugin instance of cd"};
    Itf_Player* playerCD = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplCD", id2));
    if (playerCD) {
        playerCD->play("play cd...");
    }
}

TEST_F(coreTest, unload_specail_plugin)
{
    string plugin_filename = _s_cwd + string("/plugins/libcranepluginplayermp3.so");
    cout << "plugin filename: " << plugin_filename<<endl;
    pPluginFrame->unload("Itf_Player", "PlayerImplMP3");
    pPluginFrame->unload("Itf_Player", "PlayerImplCD");
}

TEST_F(coreTest, reload_special_plugin) {
    string plugin_filename;
    plugin_filename = _s_cwd + string("/plugins/libcranepluginplayermp3.so");
    pPluginFrame->load(plugin_filename);

    plugin_filename = _s_cwd + string("/../lib/plugins/libcranepluginplayercd.so");
    pPluginFrame->load(plugin_filename);

    string id1 {"my first plugin instance of mp3"};
    Itf_Player* playerMP3 = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplMP3", id1));
    if (playerMP3) {
        playerMP3->play("play mp3...");
    }

    string id2 {"my first plugin instance of cd"};
    Itf_Player* playerCD = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplCD", id2));
    if (playerCD) {
        playerCD->play("play cd...");
    }
}
