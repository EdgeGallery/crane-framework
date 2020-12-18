
#ifndef __PLUGIN_SINGLETONINTERFACE_H__
#define __PLUGIN_SINGLETONINTERFACE_H__

#include "PluginBaseInterface.h"

class Itf_Singleton : public NS_CRANE::PluginBaseInterface {
    public:
        virtual unsigned setName(std::string&) = 0;
        virtual unsigned showName() = 0;
};

#endif