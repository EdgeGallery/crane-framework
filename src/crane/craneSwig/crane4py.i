/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-29 14:34:48
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-29 14:37:44
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
extern crane::PluginBaseInterface* create(std::string itfName, std::string pluginName, std::string description);
extern void destory(crane::PluginBaseInterface* cranePlugin);
extern void* create(std::string factoryName, std::string pluginName);
extern void destory(void *);
extern unsigned load(const std::string& filename); 
extern void unload(const std::string& type, const std::string& pluginName);