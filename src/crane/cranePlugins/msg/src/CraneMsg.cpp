/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-10 17:57:12
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-10 17:57:13
 */
#include "CraneMsg.h"

namespace NS_CRANE {
    const CraneMsg::Priority CraneMsg::DEFAULT_PRIORITY = 128;
    const time_t CraneMsg::DEFAULT_AGING_TIME = 60;//second
}