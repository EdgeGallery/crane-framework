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

#ifndef __CRANE_ABSPLUGINFRAME_H__
#define __CRANE_ABSPLUGINFRAME_H__

#include <string>
#include <iostream>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "craneMacro.h"
#include "Log.h"
#include "Util.h"
#include "PluginBaseInterface.h"
//#include "RestServer.h"

using namespace std;
using namespace Pistache;

namespace NS_CRANE {
    class AbstractPluginFrame {
        public:
            /**
             * @Descripttion: 创建插件框架的单例
             * @Param: null 
             * @Return: AbstractPluginFrame*
             */            
            static AbstractPluginFrame* getPluginFrame();
            /**
             * @Descripttion: 初始化插件系统
             * @Param {type} 
             * @Return: 
             */
            virtual unsigned init(int, char**, unsigned) = 0;
            
            /**
             * @Descripttion:   创建Crane插件的实例
             * @Param:          itfType: 插件接口类型 
             * @Param:          pluginName: 插件名称
             * @Param[out]:     ppPluginObj: 插件对象的指针的指针 
             * @Return:         CRANE_SUCC/CRANE_FAIL 
             */
            virtual PluginBaseInterface* create(const string& type, const string& pluginName, const string& name) = 0;

            virtual void destory(PluginBaseInterface*) = 0;
            /**
             * @Descripttion: 创建Gstreamer插件的实例
             * @Param:          gstFactoryName: gstreamer插件工厂名称
             * @Param:          name: 插件实例名称
             * @Param[out]:     pluginObj: 插件对象指针的指针
             * @Return:         CRANE_SUCC/CRANE_FAIL 
             */        
            virtual void* create(const string& gstFactoryName, const string& name) = 0;

            virtual void destory(void*) = 0;

            /**
             * @Descripttion: 
             * @Param: filename: Absolute filename of the library of the plugin.
             * @Return: 
             */            
            virtual unsigned load(const string& filename) = 0;

            /**
             * @Descripttion: 
             * @Param: 
             * @Return: 
             */            
            virtual void unload(const string& type, const string& pluginName) = 0;

            virtual ~AbstractPluginFrame() {std::cout<<"~AbstractPluginFrame()"<<std::endl;}
            
        private:
            class Garbo {

                public:
                    ~Garbo() {
                        if (AbstractPluginFrame::_instance != nullptr) {
                            std::cout<<"~Garbo()"<<std::endl;
                            delete AbstractPluginFrame::_instance;
                        }
                    }                
            };

        protected:
            AbstractPluginFrame();

            //_server is just used by app server thread, so it is a unique_ptr. 
            //unique_ptr<RestServer> _server{new RestServer()};    //app server instance.
            //shared_ptr<Server> _server = make_shared<Server>();

        private:
            static AbstractPluginFrame* _instance;
            
            static Garbo                _garbo;
    };

    
}

#endif
