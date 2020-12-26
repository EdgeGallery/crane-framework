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

#include "wrapper4c.h"
//#include "RestServer.h"

//using namespace NS_CRANE;

//////////////////////////////////////////////////////////////////////
// This file is used to wrap the callback function to  C signtured.
// So gstreamer plugin could invoke this function to send result data 
// back to handler function of http server which is waiting for.
// BUT this partion of code SHOULD NOT in crane framework project.
// It should put into the project of application.
// So I WILL delete the cpp and its h file later. 
// 2020-11-17
//////////////////////////////////////////////////////////////////////
void wrapperGstPluginCallback(char* key, unsigned char* result) {
    //gstPluginCallback(key, result); //TODO: delete temporarily 9-12.    

    //unused(key); unused(result);
    key = key; result = result;
}

void wrapperTestFunc() {
    //testFunc(); //TODO: delete temporarily 9-12
}
