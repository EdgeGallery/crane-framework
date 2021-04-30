/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-04-01 09:12:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-04-07 20:49:48
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


/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-04-01 09:12:15
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-04-01 09:40:43
 */

#include "CraneDisruptor.h"
#include "CraneDisruptorEvent.h"

namespace NS_CRANE {

CRANE_PLUGIN_DEFINE_WITH_T("1.0.0",
                    Itf_CraneDisruptor,
                    "1.0.0",
                    CraneDisruptor,
                    DisruptorEvent,
                    "crane system plugin, which is a high perfermence message bus.",
                    "dongyin@huawei.com",
                    "LGPL")

}