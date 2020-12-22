/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-15 11:18:48
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-10-12 18:58:56
 */ 

#ifndef __GSTFACADE_H__
#define __GSTFACADE_H__

extern "C" {
    #include <gst/gst.h>
}

#include <string>

#include "craneMacro.h"

namespace NS_CRANE {

class GstFacade {
    public:
        GstFacade() = default;
        
        unsigned gstInit(int argc, char** argv);

        void* gstCreate(std::string gstFactoryName, std::string name);
        
        void gstDestory(gpointer gstPlugin);
};

}

#endif