/*
 * @Descripttion: Create crane plugin and descory it.
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-22 11:33:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-21 17:11:13
 */

//***Include crane h file.
#include "crane.h"

//***Include plugin h file.
#include "playerInterface.h"

using namespace NS_CRANE;

int main(int argc, char** argv) {
    //*** Create a singleton object of plugin frame.
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();

    //*** Initialize Crane system only.
    pPluginFrame->init(argc, argv, CRANE_CRN);

    //*** Create "PlayerImplCD" instance of crane plugin.
    Itf_Player* playerCD = dynamic_cast<Itf_Player*>(
        pPluginFrame->create("Itf_Player", "PlayerImplCD", "my first plugin instance"));
    
    //*** Using the "PlayerImplCD" plugin instance.
    if (playerCD == nullptr) {
        return 0;
    }
    playerCD->play("Playing now......");

    //*** Release "PlayerImplCD" plugin instance.
    pPluginFrame->destory(playerCD);
    return 0;
}