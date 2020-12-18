#include "playImplCD.h"
#include "wrapplaycd.h"

/*
unsigned playcd_play(PlayerImplCD* playcd, std::string str) {
    playcd->play(str);
    return 0;
}
*/

unsigned playcd_play(crane::PluginBaseInterface* playcd, std::string str) {
    dynamic_cast<PlayerImplCD*>(playcd)->play(str);
    return 0;
}