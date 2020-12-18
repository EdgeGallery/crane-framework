/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-07-10 15:27:37
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-07-15 20:22:24
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