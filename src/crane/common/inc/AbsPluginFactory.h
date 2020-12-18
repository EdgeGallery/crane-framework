/*
 * @Descripttion: 插件工厂抽象类，各个插件通过CRANE_PLUGIN_DEFINE宏生成插件的工厂类
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-11 20:22:31
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-06-17 11:46:00
 */ 
#ifndef __CRANE_ABSPLUGINFACTORY_H__
#define __CRANE_ABSPLUGINFACTORY_H__

//#define __x86_64__

#include <string>
#include <mutex>

#include "craneMacro.h"
#include "PluginBaseInterface.h"

using namespace std;

namespace NS_CRANE {

///////////////////////////////////////////////////////////////////////////////////
/// \brief  插件工厂的抽象类，其中包含了插件元数据和工厂方法。
///         插件开发者继承该类，并实现create这个工厂方法。
///         create的返回的插件的基类接口
///////////////////////////////////////////////////////////////////////////////////
//template<class T>
//typename std::enable_if<std::is_base_of<PluginBaseInterface, T>::value, ReturnType>::type
class AbstractPluginFactory {
    public:
        /**
         * @Descripttion: 
         * @Param: 
         * @Return: 
         */        
        AbstractPluginFactory(string itfType, string pluginName, string version) : interfaceType(itfType), pluginName(pluginName), version(version) {}

        /**
         * @Descripttion: 插件的工厂方法，各个插件的工厂类实现该接口
         * @Param: null
         * @Return: 继承PluginBaseInterface接口的插件类实例的指针
         */        
        virtual PluginBaseInterface* create() = 0; 
        //virtual T* create() = 0;

        /**
         * @Descripttion: 插件的初始化函数是否已经调用，对于单例插件，初始化函数的调用次数有该方法控制
         * @Param: null 
         * @Return: bool, 是否允许进行插件实例的初始化
         */        
        virtual bool isAllowInit() = 0; 

        /**
         * @Descripttion: 获取插件所实现的应用接口名称，也就是该工厂方法创建的实例所实现的应用接口
         * @Param: null
         * @Return: 接口类名称
         */        
        const string& getInterfaceType() const;


        /**
         * @Descripttion: 获取插件工厂所创建的插件类实例的的插件类名称
         * @Param: null
         * @Return: 插件类名称
         */        
        const string& getPluginName() const;

        /**
         * @Descripttion: 获取插件所实现的接口的版本
         * @Param: null
         * @Return: 返回插件接口的版本信息
         */        
        const string& getVersion() const;
    protected:
        //插件所实现的接口名称，该名称和接口定义的名称相同
        string interfaceType = "";

        //插件接口实现类的名称，该名称和插件类的名称相同
        string pluginName = "";

        //插件接口的版本
        string version = "";

        //单例插件的互斥锁
        static mutex SingletonLock;
};

}

#endif