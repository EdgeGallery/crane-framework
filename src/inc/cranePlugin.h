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

#ifndef __CRANE_CRANEPLUGIN_H__
#define __CRANE_CRANEPLUGIN_H__

#include <memory>
#include <mutex>

#include "PluginBaseInterface.h"
#include "PluginSwappable.h"
#include "AbsPluginFactory.h"

using namespace std;
/////////////////////////////////////////////
/// \brief 插件信息
////////////////////////////////////////////
typedef struct pluginDesc{
    string fwVer;           
    string itfType;
    string itfVer;
    string pluginName;
    string desc;
    string author;
    string license;

    operator bool() const {
        return (!fwVer.empty()) && 
               (!itfType.empty()) &&
               (!itfVer.empty()) &&
               (!pluginName.empty()) &&
               (!desc.empty()) &&
               (!author.empty()) &&
               (!license.empty());
    }
}PluginDesc;

#define CRANE_STRIFY(a) #a
#define CRANE_PASTE_ARGS(a,b) a##b
#define CRANE_PASTE(a,b)    CRANE_PASTE_ARGS(a,b)

#define CRANE_PLUGIN_FACTORY_SINGLETON(itfType, itfVer, pluginName) \
class pluginName##Factory : public crane::IPluginFactory { \
    public: \
        itfType * create() { \
            lock_guard<mutex> guard(_lock); \
            if (nullptr == _instance()) { \
                _instance() = new pluginName(); \
                *(_allowInit()) = true; \
            } else { \
                *(_allowInit()) = false; \
            } \
            return _instance(); \
        } \
\
        pluginName##Factory() : crane::IPluginFactory(#itfType, #pluginName, itfVer) { } \
\
        bool isAllowInit() { return *(_allowInit()); } \
    private: \
        static itfType*& _instance() { \
            static itfType * instance = nullptr; \
            return instance; \
        } \
        bool* _allowInit() { \
            static bool allowInitilaze = false; \
            return &allowInitilaze; \
        } \
        static mutex _lock; \
\
}; \
mutex pluginName##Factory::_lock;

#define CRANE_PLUGIN_DESC(fwVer, itfType, itfVer, pluginName, desc, author, license) \
const PluginDesc pluginDesc = { \
    fwVer,\
    CRANE_STRIFY(itfType),\
    itfVer,\
    CRANE_STRIFY(pluginName),\
    desc,\
    author,\
    license\
};

#define CRANE_PLUGIN_DESC_FUNC \
extern "C" const PluginDesc* crane_plugin_desc(void) \
{\
    return &pluginDesc; \
} \

#define CRANE_PLUGIN_FACTORY_FUNC(pluginName) \
extern "C" shared_ptr<crane::IPluginFactory> createPluginFactory() {\
    return make_shared<pluginName##Factory>();\
}

#define CRANE_PLUGIN_DEFINE_SINGLETON(fwVer, itfType, itfVer, pluginName, desc, author, license) \
    CRANE_PLUGIN_FACTORY_SINGLETON(itfType, itfVer, pluginName) \
    CRANE_PLUGIN_DESC(fwVer, itfType, itfVer, pluginName, desc, author, license) \
    CRANE_PLUGIN_DESC_FUNC \
    CRANE_PLUGIN_FACTORY_FUNC(pluginName)



///////////////////////////////////////////////////////////////////////
/// \brief  插件开发者为一个插件接口的实现调用该宏，生成插件实现类的工厂类
///         插件框架调用。。。
///         TODO:后续需要支持在一个so中提供多个插件接口的实现
///         fwVer:      插件框架的最低版本要求，如1.0.1
///         itfType:    插件接口名称，如：Itf_Player，和代码的接口定义同名
///         itfVer:     插件接口版本，如1.2.1
///         pluginName: 插件接口实现类的名称，如：PlayerImplCD 
///         desc:       插件实现描述 
///         author:     插件作者，如：hwdy
///         license:    插件license
///////////////////////////////////////////////////////////////////////

#define CRANE_PLUGIN_DEFINE(fwVer, itfType, itfVer, pluginName, desc, author, license) \
class pluginName##Factory : public crane::IPluginFactory { \
    public: \
        pluginName * create() { return new pluginName(); } \
        pluginName##Factory() : crane::IPluginFactory(#itfType, #pluginName, itfVer) { } \
        bool isAllowInit() { return true; } \
}; \
\
const PluginDesc pluginDesc = { \
    fwVer,\
    CRANE_STRIFY(itfType),\
    itfVer,\
    CRANE_STRIFY(pluginName),\
    desc,\
    author,\
    license\
};\
extern "C" const PluginDesc* crane_plugin_desc(void) \
{\
    return &pluginDesc; \
} \
extern "C" shared_ptr<crane::IPluginFactory> createPluginFactory() {\
    return make_shared<pluginName##Factory>();\
}

///////////////////////////////////////////////////////////////////////
/// \brief  Developer of the pluign should invoke the macro function to generate
///         the struction of the plugin information, the factory class and 
///         factory method of the plugin.
///         插件框架调用。。。
///         fwVer:      framework version which is depended by the plugin.
///         itfType:    interface name of plugin, e.g. Itf_Player
///         itfVer:     version of the plugin implemention，如1.2.1
///         pluginName: class name of the implemention of interface. e.g PlayerImplCD 
///         T:          template parameter of the plugin implement class.
///         desc:       description of the plugin. 
///         author:     author of the plugin, e.g hwdy
///         license:    license of the plugin
///////////////////////////////////////////////////////////////////////
#define CRANE_PLUGIN_DEFINE_WITH_T(fwVer, itfType, itfVer, pluginName, T, desc, author, license) \
class pluginName_T_Factory : public crane::IPluginFactory { \
    public: \
        pluginName<T> * create() { return new pluginName<T>(); } \
        pluginName_T_Factory() : crane::IPluginFactory(#itfType, #pluginName"<"#T">", itfVer) { } \
        bool isAllowInit() { return true; } \
}; \
\
const PluginDesc pluginDesc = { \
    fwVer,\
    CRANE_STRIFY(itfType),\
    itfVer,\
    #pluginName"<"#T">",\
    desc,\
    author,\
    license\
};\
extern "C" const PluginDesc* crane_plugin_desc(void) \
{\
    return &pluginDesc; \
} \
extern "C" shared_ptr<crane::IPluginFactory> createPluginFactory() {\
    return make_shared<pluginName_T_Factory>();\
}
#endif
