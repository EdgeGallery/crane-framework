/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-03-08 17:28:29
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

#include "fetchImgInfo.h"

unsigned FetchImgInfo::init() {
    if (!Py_IsInitialized()) { 
        cout<<"FetchImgInfo: Initialize python"<<endl;
        Py_Initialize(); }

    if (!Py_IsInitialized()) { 
        cout<<"FetchImgInfo: Initialize python failed"<<endl;
        return 1;
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    // 载入名为pytest的脚本
    PyObject *pName,*pModule,*pDict;
    pName = PyUnicode_FromString("dongyin");
    pModule = PyImport_Import(pName);
    if ( !pModule ) {
        cout<<"FetchImgInfo: Can't find dongyin.py"<<endl;
        return 1;
    }
    pDict = PyModule_GetDict(pModule);
    if ( !pDict ) {
        cout<<"FetchImgInfo: Can't get func dict."<<endl;
        return 1;
    }

    _pFunc = PyObject_GetAttrString(pModule, "wang");

    return CRANE_SUCC;
}

unsigned FetchImgInfo::start() {
    cout << "Crane plugin: FetchImgInfo plugin is start..." << endl;
    return CRANE_SUCC;
}

unsigned  FetchImgInfo::stop() noexcept {
    cout << "Crane plugin: FetchImgInfo plugin is stop..." << endl;
    return CRANE_SUCC;
}

unsigned FetchImgInfo::fetchInfo(array<float, 4096>& data, float longitude, float latitude ,Result& result) {

    PyObject* args = PyTuple_New(3);
    PyObject *PyList  = PyList_New(0);

    PyObject* arg1 = Py_BuildValue("f", longitude);
    PyObject* arg2 = Py_BuildValue("f", latitude);

    for(auto f : data) {
        PyList_Append(PyList, Py_BuildValue("f", f));
    }

    PyTuple_SetItem(args, 0, PyList);
    PyTuple_SetItem(args, 1, arg1);
    PyTuple_SetItem(args, 2, arg2);  

    PyObject *ReturnTuple = PyObject_CallObject(_pFunc, args);
    char* pic_name;
    float gps_diff = 0;
    float gps_count = 0;
    float gps_time = 0;
    PyArg_ParseTuple(ReturnTuple, "sfff", &pic_name, &gps_diff, &gps_count, &gps_time);  

    result.picName = string(pic_name);
    result.gpsDiff = gps_diff;
    result.gpsCount = gps_count;
    result.gpsTime = gps_time;

    return 0;
}

CRANE_PLUGIN_DEFINE_SINGLETON("1.0.0",    //插件接口版本
                    Itf_FetchInfo,
                    "1.0.1",    //要求的插件框架版本   
                    FetchImgInfo,
                    "fetch info of the img.",
                    "testuser",
                    "LGPL")
