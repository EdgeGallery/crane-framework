/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-10-04 19:36:53
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-14 16:44:56
 */
#include "playImplCD.h"
#include "wrapplaycd.h"

unsigned playcd_play(crane::PluginBaseInterface* playcd, std::string str) {
    dynamic_cast<PlayerImplCD*>(playcd)->play(str);
    return 0;
}