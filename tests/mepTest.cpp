/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-10 20:59:16
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-10 21:42:39
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

    Itf_CraneMep* mep = dynamic_cast<Itf_CraneMep*>(pPluginFrame->create("Itf_CraneMep", "CraneMep", "Crane MEP."));

    cout<<"mep { "<<mep<<" }"<<endl;

    mep->invoke();
}