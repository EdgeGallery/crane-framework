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

#include "CraneMep.h"

using namespace std;

namespace NS_CRANE {
    const string CraneMep::CONFIG_FILENAME = ".crane_plugin_mep.cnf";

    unsigned int CraneMep::init() {
        LOG_ENTER();
        unsigned ret = _parseConfigFile();
        if (ret != CRANE_SUCC) {
            return ret;
        }

        _mepMediator = make_shared<MepMediator>();
        dynamic_pointer_cast<MepMediator>(_mepMediator)->stuffRegService(_regServices)
                        .stuffSubServices(_subServices)
                        .stuffDsvServices(_dsvServices);

        _mepMediator->CreateMepServices();
        return CRANE_SUCC;
    }

    unsigned CraneMep::invoke() {
        _mepMediator->doAction();
        return CRANE_SUCC;
    }

    unsigned CraneMep::doDiscover() {
        _mepMediator->doDiscover();
        return CRANE_SUCC;
    }
    
    const DiscoverServicesResult& CraneMep::fetchServices() {
        return _mepMediator->discoverServicesResult();
    }
    
    const ServicesSubscriberInfo& CraneMep::fetchSubNtfInfo() {
        return _mepMediator->servicesSubscriberInfo();
    }

    unsigned CraneMep::_parseConfigFile() {
        LOG_ENTER();
        const char* homepath = getenv("HOME");
        if (homepath == NULL) { 
            LOG_ERROR("Cannot find the HOME env");
            return CRANE_FAIL; 
        }

        string file = string(homepath) + "/" + CONFIG_FILENAME;
        FILE *f = fopen(file.c_str(), "r");
        if (f == NULL) {
            LOG_ERROR("Open mep config file { %s } failed.", file.c_str());
            return CRANE_FAIL;
        }

        char buf[1024] = "";
        FileReadStream is(f, buf, sizeof(buf));
        
        Document d;
        d.ParseStream<0, UTF8<>, FileReadStream>(is);
        if (d.HasParseError()) {
            LOG_ERROR("Parse error: { %d : %lu } %s.", 
            d.GetParseError(), 
            d.GetErrorOffset(),  
            (char*)rapidjson::GetParseErrorFunc(d.GetParseError()));
            return CRANE_FAIL;    
        }

        list<string> h;
        //fatch service discover list.
        h = {"dsvServices"};
        Value* v = Util::fatchJsonValue(d, h);
        if ((v != nullptr) && v->IsArray()) {
            for (auto itr = v->Begin(); itr != v->End(); ++itr) {
                _dsvServices.push_back(string(itr->GetString()));
            }
        }

        //fatch service register list.
        h = {"regServices"};
        v = Util::fatchJsonValue(d, h);
        if ((v != nullptr) && v->IsArray()) {
            for (auto itr = v->Begin(); itr != v->End(); ++itr) {
                _regServices.push_back(string(itr->GetString()));
            }
        }

        //fatch service subscriber list.
        h = {"subServices"};
        v = Util::fatchJsonValue(d, h);
        if ((v != nullptr) && v->IsArray()) {
            for (auto itr = v->Begin(); itr != v->End(); ++itr) {
                _subServices.push_back(string(itr->GetString()));
            }
        }

        return CRANE_SUCC;
    }
    
    CRANE_PLUGIN_DEFINE_SINGLETON("1.0.0",    //插件接口版本
                    Itf_CraneMep,
                    "1.0.0",    //要求的插件框架版本   
                    CraneMep,
                    "crane system plugin, which interactive with MEP.",
                    "dongyin@huawei.com",
                    "LGPL")
}
