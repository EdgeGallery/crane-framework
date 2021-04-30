/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-30 09:36:46
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

#include "gtest/gtest.h"

#include "crane.h"
#include "Util.h"

#include "CraneMep.h"

using namespace std;
using namespace NS_CRANE;

TEST(MEP, MepBaisc) {
    cout<<"Enter MepBasic()"<<endl; 
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
    int argc = 1;
    char **argv = nullptr;
    pPluginFrame->init(argc, argv, CRANE_CRN);
    string id {"Crane MEP"};
    Itf_CraneMep* mep = dynamic_cast<Itf_CraneMep*>(pPluginFrame->create("Itf_CraneMep", "CraneMep", id));

    cout << "mep { " << mep << " }" << endl;

    mep->invoke();
}
