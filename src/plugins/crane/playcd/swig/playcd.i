/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: hwdy
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: hwdy
 * @LastEditTime: 2021-03-27 09:07:23
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

%module craneplaycd 

//////////////////////////////////////////////////
%include "std_string.i"

%{
#include "playImplCD.h"
#include "wrapplaycd.h"
%}

//////////////////////////////////////////////////

extern unsigned playcd_play(crane::PluginBase* playcd, std::string str);
