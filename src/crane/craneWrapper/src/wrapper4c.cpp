/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-12 11:36:26
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-11-07 13:20:47
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
