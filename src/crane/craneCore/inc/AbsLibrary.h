/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-10 17:01:04
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-17 15:17:40
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