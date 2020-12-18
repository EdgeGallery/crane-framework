#ifndef __CRANEDATA_H__
#define __CRANEDATA_H__

#include <iostream>
#include <map>
#include <array>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <pistache/http.h>
#include <pistache/router.h>

#include "craneMacro.h"
#include "Log.h"
#include "Util.h"

using namespace std;

namespace NS_CRANE {
    //template <typename D>
            
    class Data {

        public:
            using DataMap = map<std::string, shared_ptr<Data>>;
            using ResultData = array<float, 4096>;
            static DataMap s_data;
            static mutex    s_data_mtx;

            mutex& getMutex();

            condition_variable& getCv();

            bool isReady();

            ResultData& getData();

            void setKey(string& key);


            void setData(ResultData& data);

            //void postProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

        private:
            void setReady(bool ready);

        private:
            //string              _data;
            ResultData          _data;       
            mutex               _mtx;
            condition_variable  _cv;
            string              _key;
            bool                _ready = false;
    };
    //using DataMap = map<std::string, shared_ptr<Data>>;
}

#endif 