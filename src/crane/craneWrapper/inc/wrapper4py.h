/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-29 14:28:27
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-29 14:28:27
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