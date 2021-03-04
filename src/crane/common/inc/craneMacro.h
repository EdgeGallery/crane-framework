/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-04 17:51:24
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

#ifndef __CRANE_CRANE_MACRO_H__
#define __CRANE_CRANE_MACRO_H__

///////////////////////////////////////////////////////////
///插件架构版本号
///////////////////////////////////////////////////////////
#define CRANE_FRAMEWORK_VERION "1.0.1"
#define CRANE_VERSION_LENGTH    3

///////////////////////////////////////////////////////////
///插件架构类型
///////////////////////////////////////////////////////////
#define CRANE_CRN   0     //只支持CRANE插件架构
#define CRANE_GST   1     //只支持Gstreamer插件架构
#define CRANE_ALL   2     //支持Crane和Gstreamer两种插件架构

///////////////////////////////////////////////////////////
///插件架构启动方式
///////////////////////////////////////////////////////////
#define CRANE_INIT_LIB_FILE             "LIB"
#define CRANE_INIT_CACHE_FILE           "CACHE"

///////////////////////////////////////////////////////////
///插件动态库文件相关的宏
///////////////////////////////////////////////////////////
#define CRANE_PLUGIN_LIB_PREFIX             "libcraneplugin"
#define CRANE_PLUGIN_LIB_SUFFIX             "so" 
#define CRANE_PLUGIN_DESC_FUNC_SYMBOL       "crane_plugin_desc"
#define CRANE_PLUGIN_FACTORY_FUNC_SYMBOL    "createPluginFactory"

///////////////////////////////////////////////////////////
///插件系统的命名空间
///////////////////////////////////////////////////////////
#define NS_CRANE        crane

///////////////////////////////////////////////////////////
///插件系统的命名空间
///////////////////////////////////////////////////////////
#define CRANE_CONFIG_FILENAME       ".crane.cnf"

///////////////////////////////////////////////////////////
///网络相关的宏
///////////////////////////////////////////////////////////
#define HTTPS                       "https://"
#define HTTP                        "http://"


///////////////////////////////////////////////////////////
/// Length of LINUX UUID
///////////////////////////////////////////////////////////
#define UUID_LEN 64

///////////////////////////////////////////////////////////
/// NULL PLUGIN ID
///////////////////////////////////////////////////////////
#define NULL_PLUGIN_ID "__NULL_PLUGIN_INSTANCE_ID__"

///////////////////////////////////////////////////////////
///和C语言混合编译的相关宏
///////////////////////////////////////////////////////////
#define GST_NULLPTR     ((void*)0)

///////////////////////////////////////////////////////////
///函数返回值
///////////////////////////////////////////////////////////
#define CRANE_SUCC      0
#define CRANE_FAIL      1

#define CRANE_EQUAL     0
#define CRANE_HIGHER    1
#define CRANE_LOWER     -1

#endif
