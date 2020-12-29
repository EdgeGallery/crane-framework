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
