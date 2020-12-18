#ifndef __PLUGIN_FETCHINFOINTERFACE_H__
#define __PLUGIN_FETCHINFOINTERFACE_H__

#include <array>
#include "PluginBaseInterface.h"

using namespace std;

class Itf_FetchInfo : public NS_CRANE::PluginBaseInterface {
    public:
        typedef struct {
            std::string     picName;
            float           gpsDiff;
            float           gpsCount;
            float           gpsTime;
        } Result;

    public:
        virtual unsigned fetchInfo(std::array<float, 4096>&, float longitude, float latitude ,Result&) = 0;
};

#endif