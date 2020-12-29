/*
 *    Copyright 2020 Huawei Technologies Co., Ltd.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
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
