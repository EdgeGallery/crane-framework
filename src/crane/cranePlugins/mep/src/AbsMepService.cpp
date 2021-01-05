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

#include "AbsMepService.h"

namespace NS_CRANE {

    ErrorRspBody AbstractMepService::parseRspErrorBody(const string& body) {
        Document d;
        d.Parse(body.c_str());
        ErrorRspBody errBody;
        Value* v;
        list<string> h;

        h = {"type"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return errBody;}
        errBody.type = string(v->GetString());

        h = {"title"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return errBody;}
        errBody.title = string(v->GetString());

        h = {"status"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return errBody;}
        errBody.status = v->GetUint();

        h = {"detail"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return errBody;}
        errBody.detail = string(v->GetString());

        h = {"instance"};
        v = Util::fatchJsonValue(d, h);
        if (v == nullptr) {return errBody;}
        errBody.instance = string(v->GetString());
        
        return errBody;
    }
}
