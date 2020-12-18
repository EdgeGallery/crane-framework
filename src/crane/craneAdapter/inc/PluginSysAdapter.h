/*
 * @Descripttion: CRANE插件框架的适配类，实现AbstractPluginFrame接口的同时，采用类适配方式适配CRANE和GSTREAMER两个插件系统
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-09 16:49:40
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-29 14:25:55
 */ 

#ifndef __CRANE_PLUGINSYSADAPTER_H__
#define __CRANE_PLUGINSYSADAPTER_H__

#include <cstdio>
#include <string>
#include <memory>
#include <iostream>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "craneMacro.h"
#include "Config.h"
#include "Util.h"
#include "Log.h"
#include "AbsPluginFrame.h"
#include "GstFacade.h"
#include "CraneRegistry.h"

#include "CraneMsgCenterInterface.h"

#ifdef BUILD_IMG 
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "Data.h"
#include "ImgData.h"
#include "restServerProcess.h"
#endif

using namespace std;


namespace NS_CRANE {

    class PluginSysAdapter : public AbstractPluginFrame,  private CraneRegistry, private GstFacade {
        friend class AbstractPluginFrame;

        public:
            unsigned init(int, char**, unsigned) override;
            
            /**
             * @Descripttion: Create a plugin instance though the interface type and plugin implemention class name. 
             * If the library file of the plugin implemention class has not loaded, Crane will load it depend on the 
             * absolute file name of library contained in the registry.
             * @Param:  type: Interface type of plugin, which is same as the pure virtual class name of the plugin.
             *          pluginName: Plugin class name which implement the pure virtual class name of the plugin.
             *          description: Description of this instance of the plugin.                      
             * @Return: pointer of the instance of the plugin.
             */            
            PluginBaseInterface* create(const string& type, const string& pluginName, const string& description) override;

            void destory(PluginBaseInterface*) override;

            void* create(const string& gstFactoryName, const string& name) override;

            void destory(void*) override;

            unsigned load(const string& filename) override; 
            /**
             * @Descripttion: Unload the plugin library from virtual memory space of the process.
             * @Param:  type: Interface type of plugin, which is same as the pure virtual class name of the plugin.
             *          pluginName: Plugin class name which implement the pure virtual class name of the plugin.
             *          description: Description of this instance of the plugin.                      
             * @Return: null
             */            
            void unload(const string& type, const string& pluginName) override;

            //unsigned _loadPluginLib(const string& filename);

        public:
            ~PluginSysAdapter() override;

        protected:
            PluginSysAdapter();

        private:
            //AbstractMediator*            _mepMediator;
            //AbstractMepService*            _mepMediator;
            //shared_ptr<AbstractMediator>   _mepMediator; //del by dongyin 2020-9-11
            //shared_ptr<MepMediator>   _mepMediator;

            shared_ptr<Config>          _config;
            //Config              _config;

            vector<string> _dsvServices;
            vector<string> _regServices;
            vector<string> _subServices;

            mutex                       _mtx;

    };
}

#endif