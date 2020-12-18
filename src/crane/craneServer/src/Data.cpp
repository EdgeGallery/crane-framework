#include "Data.h"

namespace NS_CRANE {
    //template <typename D>
    Data::DataMap Data::s_data;
    mutex Data::s_data_mtx;

    mutex& Data::getMutex() {
        return _mtx;
    }

    condition_variable& Data::getCv() {
        return _cv;
    }

    Data::ResultData& Data::getData() {
        return _data;
    }

    bool Data::isReady() {
        return _ready;
    }

    void Data::setKey(string& key) {
        _key = key;
    }

    void Data::setReady(bool ready) {
        _ready = ready;
    }

    void Data::setData(ResultData& data) {
        _data = data;
        Data::setReady(true);
    }


    //template <typename D>
    /*
    void Data::postProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
        unused(request);
        unused(response);
        return;
    }
    */


}