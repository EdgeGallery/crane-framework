/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-12 11:36:26
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-12 11:37:39
 */
#include "wrapper4c.h"
#include "RestServer.h"

using namespace NS_CRANE;

void wrapperGstPluginCallback(char* key, unsigned char* result) {
    //gstPluginCallback(key, result); //TODO: delete temporarily 9-12.    
    unused(key); unused(result);
}

void wrapperTestFunc() {
    //testFunc(); //TODO: delete temporarily 9-12
}
