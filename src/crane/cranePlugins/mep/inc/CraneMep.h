/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-10 19:37:48
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-11 18:51:32
 */
#ifndef __PLUGIN_CRANEMEP_H__
#define __PLUGIN_CRANEMEP_H__

#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

#include "Log.h"
#include "Util.h"

#include "cranePlugin.h"
#include "CraneMepInterface.h"
#include "MepMediator.h"

using namespace std;

namespace NS_CRANE {
    class CraneMep : public Itf_CraneMep {
    public:
        static const string CONFIG_FILENAME;

        CraneMep() { }

        virtual ~CraneMep() { }
        
        unsigned init() override;

        /**
         * @Descripttion: Invoke produre of interaction with mep. This method should be invoke during system has initialized.
         * @Param: null
         * @Return: CRANE_SUCC/CRANE_FAIL
         */    
        unsigned invoke() override;

        /**
         * @Descripttion: 手动触发服务发现过程，这里只是一个例子，该插件的继承类可以定义并实现其他的doXXXXXX()接口。
         * @Param: 
         * @Return: 
         */    
        virtual unsigned doDiscover() override;

        /**
         * @Descripttion: Fetch services information which have been discovered.
         * @Param: null
         * @Return: DiscoverServicesResult&: services information. 
         */    
        virtual const DiscoverServicesResult& fetchServices() override;

        /**
         * @Descripttion: Fetch services status notify information response from mep.
         * @Param: null 
         * @Return: ServicesSubscriberInfo
         */        
        virtual const ServicesSubscriberInfo& fetchSubNtfInfo() override;

    private:
        unsigned _parseConfigFile();

    private:
        shared_ptr<AbstractMediator>        _mepMediator;
        vector<string>                      _regServices;
        vector<string>                      _dsvServices;
        vector<string>                      _subServices;        
    };

}

#endif