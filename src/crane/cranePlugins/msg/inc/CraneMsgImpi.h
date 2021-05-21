/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: hwdy
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: hwdy
 * @LastEditTime: 2021-03-30 09:29:19
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
            string id{};
            static Itf_CraneMsgCenter* _msgCenter = dynamic_cast<Itf_CraneMsgCenter*>(
                AbstractPluginFrame::getPluginFrame()->create("Itf_CraneMsgCenter", 
                                                            "CraneMsgCenter", 
                                                            id));
            return _msgCenter;
        }

        T   _data;
};

}

#endif
