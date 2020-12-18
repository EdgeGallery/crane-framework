/*
 * @Descripttion: Crane message implement class.
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-09 16:53:13
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-10 16:00:44
 */

#ifndef __PLUGIN_CRANEMSGIMPI_H__
#define __PLUGIN_CRANEMSGIMPI_H__

#include <string>
#include <time.h>
#include <chrono>

#include "craneMacro.h"
#include "CraneMsgCenter.h"

using namespace std;

namespace NS_CRANE {

class CraneMsgImpi : public CraneMsg {
    public:         
        CraneMsgImpi(CraneTopic topic, Priority pri = 128, time_t ageingTime = 20) : CraneMsg(topic, pri, ageingTime) {
            LOG_ENTER();
            _msgCenter = dynamic_cast<Itf_CraneMsgCenter*>(
                AbstractPluginFrame::getPluginFrame()->create("Itf_CraneMsgCenter", "CraneMsgCenter", ""));         
        }

        Itf_CraneMsgCenter* msgCenter() {
            return _msgCenter;
        }

        virtual ~CraneMsgImpi() {}


    private:
        Itf_CraneMsgCenter*     _msgCenter;
};

}

#endif