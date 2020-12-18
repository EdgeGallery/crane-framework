/*
 * @Descripttion: 插件框架的抽象类，该类中定义插件框架需要支持的接口方法. APP面向该方法和插件框架进行交互
 * @version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-09 16:45:44
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-27 11:36:39
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
/*
            template <typename Result, typename Cls, typename... Args, typename Obj>
            void setServerRoutes(HttpMethod method, const string& path, Result (Cls::*func)(Args...), Obj obj) {
                switch (method) {
                    case HttpMethod::POST:
                        Rest::Routes::Post(_server->router, path, Rest::Routes::bind(func, obj));
                        break;

                    case HttpMethod::PUT:
                        Rest::Routes::Put(_server->router, path, Rest::Routes::bind(func, obj));
                        break;
                    
                    case HttpMethod::GET:
                        Rest::Routes::Get(_server->router, path, Rest::Routes::bind(func, obj));
                        break;
                    
                    case HttpMethod::DELETE:
                        Rest::Routes::Delete(_server->router, path, Rest::Routes::bind(func, obj));
                        break;
                    
                    default:
                        LOG_ERROR("Invalid http method[ %u ]\n", static_cast<unsigned>(method));
                        break;
                }
                return;
            }

            template <typename Result, typename Cls, typename... Args, typename Obj>
            void setServerRoutes(HttpMethod method, const string& path, Result (Cls::*func)(Args...), shared_ptr<Obj> objPtr) {
                switch (method) {
                    case HttpMethod::POST:
                        Rest::Routes::Post(_server->router, path, Rest::Routes::bind(func, objPtr));
                        break;

                    case HttpMethod::PUT:
                        Rest::Routes::Put(_server->router, path, Rest::Routes::bind(func, objPtr));
                        break;
                    
                    case HttpMethod::GET:
                        Rest::Routes::Get(_server->router, path, Rest::Routes::bind(func, objPtr));
                        break;
                    
                    case HttpMethod::DELETE:
                        Rest::Routes::Delete(_server->router, path, Rest::Routes::bind(func, objPtr));
                        break;
                    
                    default:
                        LOG_ERROR("Invalid http method[ %u ]\n", static_cast<unsigned>(method));
                        break;
                }
                return;
            }

            template <typename Result, typename... Args>
            void setServerRoutes(HttpMethod method, const string& path, Result (*func)(Args...)) {
                switch (method) {
                    case HttpMethod::POST:
                        Rest::Routes::Post(_server->router, path, Rest::Routes::bind(func));
                        break;

                    case HttpMethod::PUT:
                        Rest::Routes::Put(_server->router, path, Rest::Routes::bind(func));
                        break;
                    
                    case HttpMethod::GET:
                        Rest::Routes::Get(_server->router, path, Rest::Routes::bind(func));
                        break;
                    
                    case HttpMethod::DELETE:
                        Rest::Routes::Delete(_server->router, path, Rest::Routes::bind(func));
                        break;
                    
                    default:
                        LOG_ERROR("Invalid http method[ %u ]\n", static_cast<unsigned>(method));
                        break;
                }
                return;
            }
*/
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
            
            static Garbo _garbo;

    };

    
}

#endif