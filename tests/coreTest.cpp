/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-27 12:07:38
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-29 12:07:44
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
            cout<<"SetUpTestCase"<<endl;
            char* cwd = NULL;
            if ((cwd = getcwd(NULL, 0)) == NULL) {
                cout<<"Get current path failed."<<endl;
                free(cwd);
                return;
            } else {
                cout<<"Current path: "<<cwd<<endl;
            }

            string s_cwd(cwd);
            s_oldfile = cwd; s_newfile = cwd;
            _s_cwd = s_cwd;
            free(cwd);

            s_oldfile += string("/../lib/plugins/libcranepluginplayermp3.so");
            s_newfile += string("/plugins/libcranepluginplayermp3.so");
            cout<<"s_oldfile: "<<s_oldfile<<endl;
            cout<<"s_newfile: "<<s_newfile<<endl;

            if (access("plugins", F_OK|R_OK|W_OK|X_OK)) {
                int ret = mkdir("plugins", MODE);
                if (ret != 0) {
                    cout << "Error: " << strerror(errno) << endl;
                    return;
                }
                plugins_dir = opendir("plugins");
            }


            if (access("plugins/libcranepluginplayermp3.so", F_OK)) {
                int ret = rename(s_oldfile.c_str(), s_newfile.c_str());
                if (ret != 0) {
                    cout << "Error: " << strerror(errno) << endl;
                    return;
                }
            }

            pPluginFrame = AbstractPluginFrame::getPluginFrame();
            int argc = 1; char **argv = nullptr;
            pPluginFrame->init(argc, argv, CRANE_CRN);            
        }
        static void TearDownTestCase() {
            cout<<"TearDownTestCase"<<endl;
            if (!access("plugins/libcranepluginplayermp3.so", F_OK)) {
                if (!rename(s_newfile.c_str(), s_oldfile.c_str())) {
                    cout << "Error: " << strerror(errno) << endl;
                    return;
                }
            }
            if (!access("plugins", F_OK)) {
                closedir(plugins_dir);
                int ret = rmdir("plugins");
                if (!ret) {
                    cout << "Error: " << strerror(errno) << endl;
                }
            }
        }

        
        virtual void SetUp() {
            cout<<"SetUp()"<<endl;            
        }

        virtual void TearDown() {
            cout<<"TearDown()"<<endl;            

        }

        static string _s_cwd;
};
string coreTest::_s_cwd = "";

TEST_F(coreTest, load_specail_plugin)
{
    string plugin_filename = _s_cwd + string("/plugins/libcranepluginplayermp3.so");
    pPluginFrame->load(plugin_filename);

    Itf_Player* playerMP3 = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplMP3", "my first plugin instance"));
    if (playerMP3) { playerMP3->play("play mp3...");}

    Itf_Player* playerCD = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplCD", "my first plugin instance"));
    if (playerCD) { playerCD->play("play cd...");}
}

TEST_F(coreTest, unload_specail_plugin)
{
    string plugin_filename = _s_cwd + string("/plugins/libcranepluginplayermp3.so");
    cout<<"plugin filename: "<<plugin_filename<<endl;
    pPluginFrame->unload("Itf_Player", "PlayerImplMP3");
    pPluginFrame->unload("Itf_Player", "PlayerImplCD");
}

TEST_F(coreTest, reload_special_plugin) {
    string plugin_filename;
    plugin_filename = _s_cwd + string("/plugins/libcranepluginplayermp3.so");
    pPluginFrame->load(plugin_filename);

    plugin_filename = _s_cwd + string("/../lib/plugins/libcranepluginplayercd.so");
    pPluginFrame->load(plugin_filename);

    Itf_Player* playerMP3 = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplMP3", "my first plugin instance"));
    if (playerMP3) { playerMP3->play("play mp3..."); }

    Itf_Player* playerCD = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplCD", "my first plugin instance"));
    if (playerCD) { playerCD->play("play cd...");}
}