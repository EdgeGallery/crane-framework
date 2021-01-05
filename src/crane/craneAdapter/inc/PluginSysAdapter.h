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
            /**
             * @Descripttion: initialize the framework by parameter.
             * @Param: mode: intitialize either Crane or Gstreamer or both. 
             * @Return: CRANE_SUCC/CRANE_FAIL 
             */            
            unsigned init(int, char**, unsigned mode) override;
            
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

            /**
             * @Descripttion: Release the plugin throuth the point.
             * @Param: the pointer of the plugin instance.
             * @Return: null
             */            
            void destory(PluginBaseInterface*) override;

            /**
             * @Descripttion: Create Gestreamer plugin, this method just wrap the function of Gstreamer. 
             * @Param: gstFactoryName: the name of the factory of the plugin 
             * @Param: name: the name of the plugin 
             * @Return: pointer of the instance of the plugin. 
             */            
            void* create(const string& gstFactoryName, const string& name) override;

            /**
             * @Descripttion: Realse the Gestreamer plugin, this method just wrap the function of Gstreamer. 
             * @Param: the pointer of the plugin 
             * @Return: null 
             */            
            void destory(void*) override;

            /**
             * @Descripttion: Load the plugin throuth the absolute library file name. 
             *                This method is used after Crane has initialized.
             * @Param: filename: absolute library file name.
             * @Return: CRANE_SUCC/CRANE_FAIL 
             */            
            unsigned load(const string& filename) override; 

            /**
             * @Descripttion: Unload the plugin library from virtual memory space of the process.
             * @Param:  type: Interface type of plugin, which is same as the pure virtual class name of the plugin.
             *          pluginName: Plugin class name which implement the pure virtual class name of the plugin.
             *          description: Description of this instance of the plugin.                      
             * @Return: null
             */            
            void unload(const string& type, const string& pluginName) override;

        public:
            ~PluginSysAdapter() override;

        protected:
            PluginSysAdapter();

        private:
            shared_ptr<Config>          _config;

            mutex                       _mtx;
    };
}

#endif
