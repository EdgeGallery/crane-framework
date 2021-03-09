/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 17:17:09
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

#ifndef __CRANE_PLUGINBASEINTERFACE_H__
#define __CRANE_PLUGINBASEINTERFACE_H__

//#define __x86_64__

#include "craneMacro.h"

namespace NS_CRANE {

class PluginBaseInterface {
    public:
        /**
         * @Descripttion: Invoked after plugin instance is created by framework.
         *                Plugin developer should implement this method to initialize the plugin.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAIL
         */        
        virtual unsigned int init() = 0;

        /**
         * @Descripttion: Invoked after init() method of plugin instance is called by framework.
         *                Plugin developer should implement this method to start executive process e.g a thread.
         *                This method could be empty, it's determined by the developer’s business needs.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAIL
         */
        virtual unsigned start() = 0;

        /**
         * @Descripttion: Invoked when plugin is destory by application or swapped down is called by framework.
         *                Plugin developer should implement this method to stop executive process e.g a thread.
         *                This method could be empty, it's determined by the developer’s business needs.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAIL
         */
        virtual unsigned stop() noexcept = 0;

    public:
        virtual ~PluginBaseInterface() {};
    
};

}

#endif
