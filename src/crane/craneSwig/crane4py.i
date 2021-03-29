/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-27 09:06:14
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

%module crane4py

//////////////////////////////////////////////////
%include "std_string.i"
%include "std_vector.i"
%include "std_list.i"
%include "std_map.i"

%template(StringVector) std::vector<std::string>;
%template(UnsignedVector) std::vector<unsigned>;

%#define CRANE_CRN   0
%#define CRANE_GST   1
%#define CRANE_ALL   2
%{
#include <iostream>

extern "C" {
    #include <gst/gst.h>
}

#include "PluginBaseInterface.h"
#include "AbsPluginFrame.h"
#include "wrapper4py.h"
%}

//////////////////////////////////////////////////

extern unsigned init(unsigned init_type);
extern crane::PluginBase* create(std::string itfName, std::string pluginName, std::string description);
extern void destory(crane::PluginBase* cranePlugin);
extern void* create(std::string factoryName, std::string pluginName);
extern void destory(void *);
extern unsigned load(const std::string& filename); 
extern void unload(const std::string& type, const std::string& pluginName);
