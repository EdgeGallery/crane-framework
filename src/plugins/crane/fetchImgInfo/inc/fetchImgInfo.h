#ifndef __PLUGIN_FETCHIMGINFO_H__
#define __PLUGIN_FETCHIMGINFO_H__

#include <iostream>
#include <Python.h>
#include "fetchInfoInterface.h"
#include "cranePlugin.h"
//#include "Data.h"

using namespace std;

class FetchImgInfo : public Itf_FetchInfo {
    public:
        unsigned init() override;
        unsigned fetchInfo(std::array<float, 4096>&, float longitude, float latitude ,Itf_FetchInfo::Result&) override;

    private:
        PyObject* _pFunc;
};

#endif