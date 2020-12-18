/*
 * @Descripttion: 
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-09 16:21:48
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-15 09:50:44
 */ 
#ifndef __CRANE_ABSINTERFACEINFO_H_
#define __CRANE_ABSINTERFACEINFO_H_

//#define __x86_64__
#include <string>

using namespace std;

namespace NS_CRANE {

///////////////////////////////////////////////////////////////////////////////////
/// \brief  插件接口信息的抽象类，承载插件接口的相关信息，并定义操作接口。
///         该接口的实现类，实现对应的接口
///////////////////////////////////////////////////////////////////////////////////
class AbstractInterfaceInfo {
    protected:
        //////////////////////////////////////////////
        /// \brief 插件接口名称 
        //////////////////////////////////////////////
        string itfType_ = "";

        //////////////////////////////////////////////
        /// \brief 插件接口对插件实现的最高版本要求
        //////////////////////////////////////////////
        string currentVersion_ = 0;

        //////////////////////////////////////////////
        /// \brief 插件接口对插件实现的最低版本要求 
        //////////////////////////////////////////////
        //TODO:待删除
        string lowestVersion_ = 0;

        //////////////////////////////////////////////
        /// \brief 存储该插件接口的插件实现类的工厂类 
        //////////////////////////////////////////////
        //list<AbstractPluginFactory> pluginFactoryList;

        AbstractInterfaceInfo(string type, string currentVersion, string lowestVersion) 
                            : itfType_(type), currentVersion_(currentVersion), lowestVersion_(lowestVersion) {}
        

    public:
        virtual unsigned validateVersion(string interfaceType) = 0;

        const string& getItfType() {
            return itfType_;
        }

};

}
#endif