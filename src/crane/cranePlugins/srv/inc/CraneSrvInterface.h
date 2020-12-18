/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-09-12 09:17:03
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-04 16:30:48
 */
#ifndef __PLUGIN_CRANESRVINTERFACE_H__
#define __PLUGIN_CRANESRVINTERFACE_H__

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "PluginBaseInterface.h"

using namespace std;
using namespace Pistache;

namespace NS_CRANE {
//template <typename Result, typename... Args>
class Itf_CraneSrv : public PluginBaseInterface {
    public:
        enum class ThreadMode{
            MAIN_THREAD = 0,
            NEW_THREAD,
        };
        //template <typename Result, typename Cls, typename... Args, typename Obj>
        
        //virtual unsigned router(HttpMethod method, const string& path, Result (Cls::*func)(Args...), Obj obj) = 0;

        //template <typename Result, typename Cls, typename... Args, typename Obj>

        //void router(HttpMethod method, const string& path, Result (Cls::*func)(Args...), shared_ptr<Obj> objPtr) = 0;

       
        //void router(HttpMethod method, const string& path, Result (*func)(Args...)) = 0;

        //virtual unsigned router(HttpMethod method, const string& path, 
                            /*void (*func)(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter)) = 0;*/
                            //Pistache::Rest::Router::Result (*func)(Args...) ) = 0;

        virtual unsigned create(unsigned short port) = 0;

        virtual Http::Endpoint::Options& options() = 0;

        virtual unsigned initialize() = 0;

        virtual unsigned router(HttpMethod method, const string& path, Rest::Route::Handler) = 0;

        virtual unsigned handler() = 0;

        virtual unsigned handler(const shared_ptr<Http::Handler> handler) = 0;

        virtual unsigned serverTLS(const string& serverCert, const string& serverKey) = 0;

        virtual unsigned clientTLS(const string& CACert) = 0;
        
        virtual void start(ThreadMode mode) = 0;
        
        virtual unsigned shutdown() = 0;
        
        virtual shared_ptr<Http::Endpoint> endpoint() = 0;

};

}

#endif