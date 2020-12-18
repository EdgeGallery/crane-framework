/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-14 14:30:49
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-12 14:46:02
 */ 
#include "RestServer.h"
#include "fetchImgInfo.h"

namespace NS_CRANE {
    void RestServer::setAddr(Address addr) {
        _endpoint = make_shared<Http::Endpoint>(addr);
    }

    void RestServer::init() {
        LOG_DEBUG("Enter Server::init()");
        auto opts = Http::Endpoint::options();
        //options from configfile.
        opts.threads(static_cast<int>(_threadNum))
            .maxRequestSize(_maxRequestSize)
            .maxResponseSize(_maxResponseSize);

        //options from crane default value.
        opts.threadsName(_threadName);
        
        _endpoint->init(opts);

        //Sever tls
        if (false) {
            _endpoint->useSSL(string("/root/certfile"), string("/root/keyfile"), false);
        }
        //Client tls
        if (false) {
            _endpoint->useSSLAuth(string("ca_file"), string("ca_path"));
        }
    }

    void RestServer::start() {
        LOG_DEBUG("Enter Server::start()");
        _endpoint->setHandler(router.handler());
        _endpoint->serveThreaded();
    }

    #ifdef BUILD_IMG
    /*
    void Server::postProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
        //unused(request);
        //unused(response);

        //fatch body info
        Document d;
        d.Parse(request.body().c_str());
        list<string> h;
        Value* v; 

        //fatch "msisdn"
        h = {"msisdn"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return;}
        string msisdn = v->IsString()? string(v->GetString()) : "";

        //fatch longitude
        h = {"longitude"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return;}
        float longitude = v->IsFloat()? v->GetFloat() : 0;
        unused(longitude);

        //fatch latitude
        h = {"latitude"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return;}
        float latitude = v->IsFloat()? v->GetFloat() : 0;
        unused(latitude);

        //fatch img_binary
        h = {"img_binary"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return;}
        string img = v->IsString()? string(v->GetString()) :  "";
        size_t len; //unsigned long int == unsigned long;
        unsigned char* pImg = Util::hexstr2uint8(img, &len);
        unused(pImg);

        string uuid = Util::uuid();

        //add uuid and data object into map;
        //TODO   ;
        
        //invoke the callback func from gst plugin.
        //TODO    ;
        //unsigned func(char*, unsigned char*, unsigned long);
        //func(uuid.c_str(), pImg, len);

        //if func is return ok, else 
        shared_ptr<Data> data = make_shared<Data>();
        Data::s_data.insert(make_pair(uuid, data));

        //wait for response data from gst pileline.        
        Data::DataMap::iterator itr;    
        unique_lock<mutex> lock(data->getMutex());
        data->getCv().wait(lock, [&uuid, &itr]{
            itr = Data::s_data.find(uuid);
            return (itr != Data::s_data.end()) ? itr->second->isReady() : false;
        });//TODO 等待超时的处理！！！

        //call crane plugin interface
        Data::ResultData result;
        {   
            lock_guard<mutex> guard(Data::s_data_mtx);
            result = itr->second->getData();
            Data::s_data.erase(itr);
        }


        AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
        Itf_FetchInfo* fetchImgInfo = dynamic_cast<Itf_FetchInfo*>(
                        pPluginFrame->create("Itf_FetchInfo", "FetchImgInfo", "test fetch image info."));

        FetchImgInfo::Result result = {0};
        fetchImgInfo->fetchInfo(result, longitude, latitude, result);                        
            


        //TODO invoke crane plugin
        //

        //send http response back to UE;
        //TODO
        //
        response.send(Http::Code::Ok);
    }
    */

    void gstPluginCallback(char* key, unsigned char* result) {
        unused(result);
        string k = string(key);

        Data::ResultData d = {0};
        for (int i = 0; i < 4096; i++) {d.at(i) = result[i];}

        auto itr = Data::s_data.find(k);
        if (itr != Data::s_data.end()) {
            lock_guard<mutex> lock(itr->second->getMutex());
            //set result data into the item of map.
            //itr->second->setData(/*result*/);
            itr->second->setData(d);
            itr->second->getCv().notify_one();
        } else {
            LOG_ERROR("Cannot find item in the data map with key: { %s }", k.c_str());
        }

    }

    void testFunc() {
        cout<<"Enter testFunc"<<endl;
    }
    #endif
}