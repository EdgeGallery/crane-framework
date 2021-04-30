/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-03-08 11:38:52
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 19:54:33
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

#ifndef __CRANE_PLUGINSWAPPABLE_H__
#define __CRANE_PLUGINSWAPPABLE_H__

//#define __x86_64__
#include <memory>

#include "craneMacro.h"
#include "PluginBaseInterface.h"

using namespace std;

namespace NS_CRANE {

class PluginSwappable {
    public:
        /**
         * @Descripttion: When the plugin should support hot swapping functionality, fresh plugin developer 
         *                  should override this method. This method would be invoked by framework 
         *                  before the moment of the fresh plugin is online.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        virtual unsigned swap_up(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) = 0;

        /**
         * @Descripttion: When the plugin should support hot swapping functionality, stale plugin developer 
         *                  should override this method. This method would be invoked by framework 
         *                  after the moment of the stale plugin is offline.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        virtual unsigned swap_down(shared_ptr<PluginBase> stale, shared_ptr<PluginBase> fresh) = 0;

    public:
        virtual ~PluginSwappable() {};
    
};

}

#endif