/*
 * @Descripttion: Crane message implement class.
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-09 16:53:13
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-12 20:04:18
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
template <typename T>
class CraneMsgImpi : public CraneMsg, public enable_shared_from_this<CraneMsgImpi<T>>  {
    public:         
        CraneMsgImpi(CraneTopic topic, Priority pri = 128, time_t ageingTime = 20) : CraneMsg(topic, pri, ageingTime) {
            LOG_ENTER();
            /*
            _msgCenter = dynamic_cast<Itf_CraneMsgCenter*>(
                AbstractPluginFrame::getPluginFrame()->create("Itf_CraneMsgCenter", "CraneMsgCenter", ""));         
            */
        }

        unsigned publish() override {
            cout<<"publish()"<<endl;
            _msgCenter()->publish(topic(), dynamic_pointer_cast<CraneMsg>(this->shared_from_this()));
            return CRANE_SUCC;
        }        

        inline const T& data() const { return _data; }

        inline void data(T& d) { _data = d; }

        virtual ~CraneMsgImpi() {}

    private:
        Itf_CraneMsgCenter* _msgCenter() {
            static Itf_CraneMsgCenter* _msgCenter = dynamic_cast<Itf_CraneMsgCenter*>(
                AbstractPluginFrame::getPluginFrame()->create("Itf_CraneMsgCenter", "CraneMsgCenter", ""));         
            return _msgCenter;
        }

        T   _data;
};

}

#endif