#include "restServerProcess.h"
#include "AbsPluginFrame.h"
#include "fetchImgInfo.h"

namespace NS_CRANE {

    #ifdef BUILD_IMG
    const std::string fetchImgRspBody = "{\
        \"msisdn\": \"+8613798413185\", \
        \"timestamp\": 12345678, \
        \"all_result\": {  \
            \"search_img\": \"pipName\",  \
            \"search_error\": 1.1, \
            \"search_number\": 2.2, \
            \"search_time\": 3.3 \
        } \
    }";
    void postProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

        LOG_DEBUG("Enter postProcess");

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
        LOG_DEBUG("MSISDN: { %s }", msisdn.c_str());

        //fatch longitude
        h = {"longitude"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return;}
        float longitude = v->IsFloat()? v->GetFloat() : 0;
        LOG_DEBUG("Logitude: { %f }", longitude);

        //fatch latitude
        h = {"latitude"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return;}
        float latitude = v->IsFloat()? v->GetFloat() : 0;
        LOG_DEBUG("Latitude: { %f }", latitude);

        //fatch img_binary
        h = {"img_binary"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return;}
        string img = v->IsString()? string(v->GetString()) :  "";
        size_t len; //unsigned long int == unsigned long;
        unsigned char* pImg = Util::hexstr2uint8(img, &len);
        unused(pImg);

        string uuid = Util::uuid();
        LOG_DEBUG("UUID of this HTTP session is { %s } with length: %u",uuid.c_str(), uuid.length());

        //invoke the callback func from gst plugin.
        //TODO    ;
        //unsigned func(char*, unsigned keylen, unsigned char*, unsigned long datalen);
        //unsigned ret = func(uuid.c_str(), keylen, pImg, len);
        //if (ret != 0) {
        //    response.send(Http::Code::Request_Timeout);
        //    return;
        //}

        //if func is return ok, else 
        shared_ptr<Data> data = make_shared<Data>();
        {
            lock_guard<mutex> guard(Data::s_data_mtx);
            Data::s_data.insert(make_pair(uuid, data));
        }
        LOG_DEBUG("Insert data into s_data map");

        //wait for response data from gst pileline.        
        Data::DataMap::iterator itr;    
        unique_lock<mutex> lock(data->getMutex());
        auto status = data->getCv().wait_for(lock, std::chrono::seconds(5), [&uuid, &itr] {
            itr = Data::s_data.find(uuid);
            return (itr != Data::s_data.end()) ? itr->second->isReady() : false;
        });

        if (status == false) {
            LOG_ERROR("Rest Server request process timeout.");
            {
                lock_guard<mutex> guard(Data::s_data_mtx);
                Data::s_data.erase(itr);
            }
            response.send(Http::Code::Request_Timeout);
            return;
        } else {
            LOG_DEBUG("Receive cv.notify from another thread.");
        }

        //call crane plugin interface
        Data::ResultData gstPipelineResult;
        {   
            lock_guard<mutex> guard(Data::s_data_mtx);
            gstPipelineResult = itr->second->getData();
            Data::s_data.erase(itr);
        }

        //Invoke crane plugin
        AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();
        Itf_FetchInfo* fetchImgInfo = dynamic_cast<Itf_FetchInfo*>(
                        pPluginFrame->create("Itf_FetchInfo", "FetchImgInfo", "test fetch image info."));

        FetchImgInfo::Result result = {0};
        if (0 != fetchImgInfo->fetchInfo(gstPipelineResult, longitude, latitude, result)) {
            LOG_ERROR("Invoke FetchImgInfo plugin's fetchInfo interface failed.");
            return;
        }
        LOG_DEBUG("Pic name：{ %s }, GPS Diff: { %f }, GPS Count: { %f }, GPS Time: { %f }", 
                        result.picName.c_str(), result.gpsDiff, result.gpsCount, result.gpsTime);
            
        //send http response back to UE;
        //TODO
        //
        Document rspDoc;
        rspDoc.Parse(fetchImgRspBody.c_str());
        auto& a = rspDoc.GetAllocator();

        h = {"msisdn"};
        v = Util::fatchJsonValue(rspDoc, h);
        if (v != nullptr) {
            v->SetString(msisdn.c_str(), a);
        }

        h = {"timestamp"};
        v = Util::fatchJsonValue(rspDoc, h);
        if (v != nullptr) {
            chrono::time_point<chrono::system_clock, chrono::milliseconds> tp 
                = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now());//获取当前时间点
            time_t timestamp =  tp.time_since_epoch().count(); //计算距离1970-1-1,00:00的时间长度
            v->SetInt64(timestamp);
        }

        h = {"all_result", "search_img"};
        v = Util::fatchJsonValue(rspDoc, h);
        if (v != nullptr) {
            v->SetString(result.picName.c_str(), a);
        }

        h = {"all_result", "search_error"};
        v = Util::fatchJsonValue(rspDoc, h);
        if (v != nullptr) {
            v->SetFloat(result.gpsDiff);
        }

        h = {"all_result", "search_number"};
        v = Util::fatchJsonValue(rspDoc, h);
        if (v != nullptr) {
            v->SetFloat(result.gpsCount);
        }

        h = {"all_result", "search_time"};
        v = Util::fatchJsonValue(rspDoc, h);
        if (v != nullptr) {
            v->SetFloat(result.gpsTime);
        }
        
        response.send(Http::Code::Ok, Util::Json2String(rspDoc), MIME(Application, Json) );
    }

    void getProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
        LOG_DEBUG("Enter getProcess");
        return;
        string picName = request.param(":name").as<std::string>();
        //string pullPicFileName = 

        Http::serveFile(response, "/opt/img/" + picName, Http::Mime::MediaType(Pistache::Http::Mime::Type::Image, Pistache::Http::Mime::Subtype::Jpeg))
        .then(
            [](ssize_t bytes) {
              std::cout << "Sent " << bytes << " bytes" << std::endl;
            },
            Async::NoExcept);        


        //call 
        /*
        auto stream = response.stream(Http::Code::Ok);
        size_t picLen = 512;
        for (size_t i = 0; i < 10; ++i) {
            stream.write(pic, picLen);
            stream.flush();
        }
        stream.ends()
        */
    }
    #endif

}