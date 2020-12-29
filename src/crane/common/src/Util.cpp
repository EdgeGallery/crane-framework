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

#include "Util.h"

namespace NS_CRANE {

    void Util::cvtVer2I(const std::string ver, unsigned* iv, unsigned len)
    {
        std::string tmp("");
        unsigned j = 0;

        for(std::size_t i = 0; i < ver.size() && j < len; ++i) {
            if (ver[i] == '.') {
                iv[j++] = atoi(tmp.c_str());
            } else {
                tmp += ver[i];
            }
        }
        iv[j++] = atoi(tmp.c_str());
    }

    int Util::compareVersion(const std::string ver1, const std::string ver2)
    {
        unsigned iVer1[CRANE_VERSION_LENGTH] = {0};
        unsigned iVer2[CRANE_VERSION_LENGTH] = {0};

        cvtVer2I(ver1, iVer1, CRANE_VERSION_LENGTH);
        cvtVer2I(ver2, iVer2, CRANE_VERSION_LENGTH);

        int ret = memcmp(iVer1, iVer2, CRANE_VERSION_LENGTH);
        if (ret < 0) {
            // ver1版本低，ver2的版本高
            return CRANE_LOWER;
        } else if(ret > 0) {
            // ver1版本高，ver2的版本低
            return CRANE_HIGHER;
        } else {
            return CRANE_EQUAL;
        }
    }

    unsigned Util::parseJsonFile(const string& absoluteFilename, Document& d)
    {
        if (absoluteFilename.empty()) { 
            LOG_ERROR("Invalid absoluteFilename parameter.");
            return CRANE_FAIL;
        }
        FILE *f = fopen(absoluteFilename.c_str(), "r");
        if (f == NULL) {
            LOG_ERROR("Open mep config file { %s } failed.", absoluteFilename.c_str());
            return CRANE_FAIL;
        }

        char buf[8092] = "";
        FileReadStream is(f, buf, sizeof(buf));

        d.ParseStream<0, UTF8<>, FileReadStream>(is);
        if (d.HasParseError()) {
            LOG_ERROR("Parse error: { %d : %lu }.", 
            d.GetParseError(), 
            d.GetErrorOffset()/*,
            (char*)rapidjson::GetParseErrorFunc(d.GetParseError())*/);
            fclose(f);
            return CRANE_FAIL;
        }

        fclose(f);
        return CRANE_SUCC;
    }

    Value* Util::fatchJsonValue(Document& document, const list<string>& hierarchy)
    {
        if (document.HasParseError()) {
            LOG_ERROR("Parse error: { %d : %lu } %s \n", 
                        document.GetParseError(), 
                        document.GetErrorOffset(),  
                        (char*)rapidjson::GetParseErrorFunc(document.GetParseError()));
            return nullptr;
        }

        Document::MemberIterator itr;
        Value* value = nullptr;

        for (auto name = hierarchy.begin(); name != hierarchy.end(); ++name) {
            if (name == hierarchy.begin()) {
                itr = document.FindMember(name->c_str());
                if (itr != document.MemberEnd()) {
                    value = &(itr->value); //---> ++name;
                } else {
                    return nullptr;
                }
            } else {
                itr = value->FindMember(name->c_str());
                if (itr != value->MemberEnd()) {
                    value = &(itr->value); 
                } else {
                    return nullptr;
                }
            } 
        }
        return value;
    }

    Value* Util::fatchJsonValue(Value& value, const list<string>& hierarchy)
    {
        Value::MemberIterator itr;
        Value* v = nullptr;

        for (auto name = hierarchy.begin(); name != hierarchy.end(); ++name) {
            if (name == hierarchy.begin()) {
                itr = value.FindMember(name->c_str());
                if (itr != value.MemberEnd()) {
                    v = &(itr->value); //---> ++name;
                } else {
                    LOG_ERROR("Cannot match the first { %s }in the hierarchy", name->c_str());
                    return nullptr;
                }
            } else {
                itr = v->FindMember(name->c_str());
                if (itr != v->MemberEnd()) {
                    v = &(itr->value); 
                } else {
                    LOG_ERROR("Cannot find value with name of the hierarchy");
                    return nullptr;
                }
            }
        }
        return v;
    }

    Value* Util::cleanJsonArray(Document& document, const list<string>& hierarchy)
    {
        Value* v = fatchJsonValue(document, hierarchy);
        if (v == nullptr) {return nullptr;}
        if (v->IsArray()) {
            v->Clear();
        }
        return v;
    }

    string Util::Json2String(Document& document)
    {
        StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        return string(buffer.GetString());
    }

    void Util::printJson(Document& document)
    {
        string str = Json2String(document);
        LOG_DEBUG("Json Info: %s", str.c_str());
    }

    string Util::writeJsonString(const rapidjson::Value& value, const std::string& name, const std::string& value1)
    {
        unused(value, name, value1);
        return std::string();
    }

    string Util::uuid()
    {
        boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        string uuid = boost::uuids::to_string(a_uuid);
        return uuid;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
    unsigned char* Util::hexstr2uint8(string& str, size_t* len)
    {
        using uint8_t = unsigned char;
        if (str.empty())
            return nullptr;


        size_t slength = str.length();
        if ((slength % 2) != 0) { // must be even
            return nullptr;
        }

        size_t dlength = slength / 2;

        unsigned char* data = (uint8_t*)malloc(dlength);

        memset(data, 0, dlength);
        *len = dlength;

        size_t index = 0;
        while (index < slength) {
            char c = str[index];
            int value = 0;
            if (c >= '0' && c <= '9') {
                value = (c - '0');
            } else if (c >= 'A' && c <= 'F') {
                value = (10 + (c - 'A'));
            } else if (c >= 'a' && c <= 'f') {
                value = (10 + (c - 'a'));
            } else {
                free(data);
                return nullptr;
            }

            data[(index / 2)] += value << (((index + 1) % 2) * 4);
            index++;
        }

        return data;
    }
#pragma GCC diagnostic pop

}
