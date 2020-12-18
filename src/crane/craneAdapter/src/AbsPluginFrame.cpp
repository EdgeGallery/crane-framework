/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-23 17:15:21
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-07-15 11:31:58
 */ 

#include "AbsPluginFrame.h"
#include "PluginSysAdapter.h"


namespace NS_CRANE {

    AbstractPluginFrame* AbstractPluginFrame::_instance = nullptr;
    AbstractPluginFrame::Garbo AbstractPluginFrame::_garbo;

    AbstractPluginFrame::AbstractPluginFrame() {
        //_server = make_shared<Server>();
        
    }
    
    AbstractPluginFrame* AbstractPluginFrame::getPluginFrame() {
        if (_instance == nullptr) {
            _instance = new PluginSysAdapter();
        }
        return _instance;
    }
}