/*
 * @Descripttion: CRANE插件应用需要include该h文件
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-19 11:56:17
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-11-07 13:43:52
 */ 
#ifndef __CRANE_CRANE_H__
#define __CRANE_CRANE_H__

//Crane basic
//#include "../crane/common/inc/craneMacro.h"
//#include "../crane/common/inc/Util.h"
#include "craneMacro.h"
#include "Util.h"

//Crane plugins
//#include "../crane/common/inc/AbsPluginFactory.h"
//#include "../crane/common/inc/PluginBaseInterface.h"
#include "AbsPluginFactory.h"
#include "PluginBaseInterface.h"

//Crane framework interfaces
//#include "../crane/craneAdapter/inc/AbsPluginFrame.h"
//#include "../crane/craneAdapter/inc/PluginSysAdapter.h"
#include "AbsPluginFrame.h"
#include "PluginSysAdapter.h"

//Crane system plugins
//#include "../crane/cranePlugins/msg/inc/CraneMsgCenterPlugin.h"
//#include "../crane/cranePlugins/mep/inc/CraneMep.h"
//#include "../crane/cranePlugins/srv/inc/CraneSrv.h"
#include "CraneMsgCenterPlugin.h"
#include "CraneMep.h"
#include "CraneSrv.h"


#endif