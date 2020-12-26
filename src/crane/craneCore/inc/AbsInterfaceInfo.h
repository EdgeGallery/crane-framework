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