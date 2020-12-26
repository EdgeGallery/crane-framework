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

#ifndef CRANE_ABSLIBRARY_H
#define CRANE_ABSLIBRARY_H

//#define __x86_64__

#include <string>

using namespace std;

namespace crane {

//////////////////////////////////////////////
/// \brief library接口，支持可能的多种library类型
//////////////////////////////////////////////
class AbsLibrary {
    public:
        //////////////////////////////////////////////
        /// \brief  动态库加载接口
        /// \param   
        /// \return library实现对象
        //////////////////////////////////////////////
        virtual AbsLibrary* load(const string& path) = 0;

        //////////////////////////////////////////////
        /// \brief  从动态库中获取符号的地址 
        /// \param  符号名称  
        /// \return 符号地址
        //////////////////////////////////////////////
        virtual void* symbol(const string& symbolName) = 0;

        //////////////////////////////////////////////
        /// \brief  获取动态库的名称，也就是动态库的文件名称
        /// \param  null  
        /// \return 动态库名称
        //////////////////////////////////////////////
        string& name();

    protected:
        //////////////////////////////////////////////
        /// \brief 动态库名称(剔除了libcrane前缀和.so后缀)
        //////////////////////////////////////////////
        string name_ = "";

        //////////////////////////////////////////////
        /// \brief 动态库句柄
        //////////////////////////////////////////////
        void* handle_ = nullptr;

};
/*
string& AbsLibrary::name() {
    return name;
}
*/
}
#endif