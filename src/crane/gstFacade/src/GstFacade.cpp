/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-15 11:17:21
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-06-20 14:58:31
 */ 


#include "GstFacade.h"

namespace NS_CRANE {
    unsigned GstFacade::gstInit(int argc, char** argv) {
        fprintf(stdout, "Init GST plugin system. \n");
        gst_init(&argc, &argv);
        return CRANE_SUCC;
    }

    void* GstFacade::gstCreate(std::string gstFactoryName, std::string name) {
        GstElement* gstPluginObj; 
        if (name.empty() == true) {
            gstPluginObj = gst_element_factory_make(gstFactoryName.c_str(), static_cast<const char*>(GST_NULLPTR));
        } else {
            gstPluginObj = gst_element_factory_make(gstFactoryName.c_str(), name.c_str()); 
        }
        return (gstPluginObj != GST_NULLPTR)? gstPluginObj : nullptr;
    } 

    void GstFacade::gstDestory(gpointer gstPlugin) {
        gst_object_unref(gstPlugin);
    }
}