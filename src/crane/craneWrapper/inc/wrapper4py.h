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

#ifndef __CRANE_WRAPPER4PY_H__
#define __CRANE_WRAPPER4PY_H__

unsigned init(unsigned init_type);

crane::PluginBaseInterface* create(std::string itfName, std::string pluginName, std::string description);

void destory(crane::PluginBaseInterface* cranePlugin);

void* create(std::string factoryName, std::string pluginName);

void destory(void *);

unsigned load(const std::string& filename); 

void unload(const std::string& type, const std::string& pluginName);

//TODO:
//unsigend setSeverHandler();


#endif