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

#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "gtest/gtest.h"

#include "Util.h"

using namespace std;
using namespace crane;

const std::string jsonBody = "{\
    \"serName\":\"ExampleService\",\
    \"serCategory\":{\
        \"href\":\"/example/catalogue1\",\
        \"id\":\"id12345\",\
        \"name\":\"RNI\",\
        \"version\":\"version1\"\
        },\
    \"version\":\"ServiceVersion1\",\
    \"state\":\"ACTIVE\",\
    \"transportId\":\"Rest1\",\
    \"transportInfo\":{\
        \"id\":\"TransId12345\",\
        \"name\":\"REST\",\
        \"description\":\"REST API\",\
        \"type\":\"REST_HTTP\",\
        \"protocol\":\"HTTP\",\
        \"version\":\"2.0\",\
        \"endpoint\":{\
            \"addresses\":[\
                    {\"host\":\"1.1.1.1\",\"port\":\"8080\"}\
                ]},\
        \"security\":{\
            \"oAuth2Info\":{\
                \"grantTypes\":[\"OAUTH2_CLIENT_CREDENTIALS\"],\
                \"tokenEndpoint\":\"/mecSerMgmtApi/security/TokenEndPoint\"}\
            },\
            \"implSpecificInfo\":{}\
        },\
    \"serializer\":\"JSON\",\
    \"scopeOfLocality\":\"MEC_SYSTEM\",\
    \"consumedLocalOnly\":false,\
    \"isLocal\":true\
    }";

TEST(jsontest, test1) {
    rapidjson::Document d;
    d.Parse(jsonBody.c_str());
    if (d.HasParseError()) {    
        fprintf(stderr,"parse error: { %d : %lu } %s \n", 
                d.GetParseError(), 
                d.GetErrorOffset(), 
                (char*)rapidjson::GetParseErrorFunc(d.GetParseError()));
    }
    ASSERT_STREQ(d["serName"].GetString(), "ExampleService");
    ASSERT_STREQ(d["state"].GetString(), "ACTIVE") << "error!!!!!!" << d["state"].GetString() << std::endl;

    auto it = d.FindMember("serName");
    ASSERT_NE(it, d.MemberEnd());
    ASSERT_TRUE(it->value.IsString());

    it = d.FindMember("isLocal");
    ASSERT_NE(it, d.MemberEnd());
    ASSERT_TRUE(it->value.IsBool()) << "error!!!!!! it is not bool." << std::endl;

    it = d.FindMember("transportInfo");
    ASSERT_NE(it, d.MemberEnd());
    ASSERT_TRUE(it->value.IsObject());
    rapidjson::Document::MemberIterator endpoint = it->value.FindMember("endpoint");
    
    ASSERT_TRUE(endpoint->value.IsObject()) << endpoint->value.GetType() << std::endl;
    ASSERT_TRUE(endpoint->value.FindMember("addresses")->value.IsArray()) << "addresses.value'type: " << endpoint->value.FindMember("addresses")->value.GetType() << std::endl;
    auto& addresses = endpoint->value.FindMember("addresses")->value;

    // rapidjson::Document::Array addrs = endpoint->value.GetArray();
    for (rapidjson::Document::ValueIterator itr = addresses.Begin(); itr != addresses.End(); ++itr) {
        ASSERT_STREQ(itr->FindMember("host")->value.GetString(), "1.1.1.1");
        ASSERT_STREQ(itr->FindMember("port")->value.GetString(), "8080");

        ASSERT_TRUE((*itr).IsObject()) << "Error!!!!!!" << (*itr).GetType() << std::endl;
        for (rapidjson::Document::ConstMemberIterator i = itr->MemberBegin(); i != itr->MemberEnd(); ++i) {\
            if (!std::string("host").compare(i->name.GetString())) {
                ASSERT_STREQ(i->value.GetString(), "1.1.1.1") << "Error!!!!!!" << i->value.GetString() << i->value.GetType() << std::endl;
            }
        }
    }

    auto result1 = crane::Util::fatchJsonValue(it->value, list<string>{"version"});
    ASSERT_STREQ(result1->GetString(), "2.0") << "Error!!!!!!" << result1->GetString() << endl;

    auto result2 = crane::Util::fatchJsonValue(d, list<string>{"version"});
    ASSERT_STREQ(result2->GetString(), "ServiceVersion1") << "Error!!!!!!" << result2->GetString() << std::endl;

    auto result3 = crane::Util::fatchJsonValue(d, list<string>{"isLocal"});
    ASSERT_TRUE(result3->IsBool()) << "Error!!!!!!" << result3->IsBool() << std::endl;
    ASSERT_TRUE(result3->GetBool()) << "Error!!!!!!" << result3->GetBool() << std::endl;

    ///////////////////////// 修改内容 ////////////////////////
    // 1. 修改transportId
    it = d.FindMember("transportId");
    std::string newTransportId = "Rest2";
    it->value.SetString(newTransportId.c_str(), static_cast<rapidjson::SizeType>(newTransportId.size()), d.GetAllocator());
    ASSERT_STREQ(it->value.GetString(), newTransportId.c_str()) << "Error!!!!!!" << it->value.GetString() << std::endl;

    // 2. 新增数组
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Value addr(rapidjson::kObjectType);
    addr.AddMember("host", "2.2.2.2", d.GetAllocator());
    addr.AddMember("port", "9090", d.GetAllocator());
    addresses.PushBack(addr, d.GetAllocator());

    // 3. 修改数组
    for (rapidjson::Document::ValueIterator itr = addresses.Begin(); itr != addresses.End(); ++itr) {
       if (itr->FindMember("host")->value == rapidjson::Value("1.1.1.1")) {
           addresses.Erase(itr);
       }
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    std::cout<<"Result Json: " << buffer.GetString() << std::endl;
}


TEST(jsontest, fatchJsonValue) {
    rapidjson::Document d;
    d.Parse(jsonBody.c_str());

    list<string> h = {"serCategory","version"};
    rapidjson::Value* v = Util::fatchJsonValue(d, h);
    ASSERT_STREQ(v->GetString(), "version1") << "version = " << v->GetString() << std::endl;

    h = {"transportId"};
    v = Util::fatchJsonValue(d, h);
    ASSERT_STREQ(v->GetString(), "Rest1") << "transportId = " << v->GetString() << std::endl;

    h = {"transportInfo","security","oAuth2Info","tokenEndpoint"};
    v = Util::fatchJsonValue(d, h);
    ASSERT_STREQ(v->GetString(), "/mecSerMgmtApi/security/TokenEndPoint") << "transportId = " << v->GetString() << std::endl;
    v->SetString("dongyin", d.GetAllocator());
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    std::cout << "Result Json: " << std::endl << buffer.GetString() << std::endl;
}

TEST(jsontest, cleanJsonArray) {
const std::string jsonBody = "{\
    \"serName\":\"ExampleService\",\
    \"serCategory\":{\
        \"href\":\"/example/catalogue1\",\
        \"id\":\"id12345\",\
        \"name\":\"RNI\",\
        \"version\":\"version1\"\
        },\
    \"version\":\"ServiceVersion1\",\
    \"state\":\"ACTIVE\",\
    \"transportId\":\"Rest1\",\
    \"transportInfo\":{\
        \"id\":\"TransId12345\",\
        \"name\":\"REST\",\
        \"description\":\"REST API\",\
        \"type\":\"REST_HTTP\",\
        \"protocol\":\"HTTP\",\
        \"version\":\"2.0\",\
        \"endpoint\":{\
            \"addresses\":[\
                    {\"host\":\"1.1.1.1\",\"port\":\"8080\"},\
                    {\"host\":\"2.2.2.2\",\"port\":\"9090\"}\
                ]},\
        \"security\":{\
            \"oAuth2Info\":{\
                \"grantTypes\":[\"OAUTH2_CLIENT_CREDENTIALS\"],\
                \"tokenEndpoint\":\"/mecSerMgmtApi/security/TokenEndPoint\"}\
            },\
            \"implSpecificInfo\":{}\
        },\
    \"serializer\":\"JSON\",\
    \"scopeOfLocality\":\"MEC_SYSTEM\",\
    \"consumedLocalOnly\":false,\
    \"isLocal\":true\
    }";

    rapidjson::Document d;
    d.Parse(jsonBody.c_str());

    list<string> h = {"transportInfo","endpoint","addresses"};
    Util::cleanJsonArray(d, h);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    std::cout << "Result Json: " << std::endl << buffer.GetString() << std::endl;
}

TEST(jsontest, parseArray) {
const std::string jsonBody = "[\
  {\
    \"serInstanceId\": \"ServiceInstance123\",\
    \"serName\": \"ExampleService\",\
    \"serCategory\": {\
      \"href\": \"catItem1\",\
      \"id\": \"id12345\",\
      \"name\": \"RNI\",\
      \"version\": \"version1\"\
    },\
    \"version\": \"ServiceVersion1\",\
    \"state\": \"ACTIVE\",\
    \"transportInfo\": {\
      \"id\": \"TransId12345\",\
      \"name\": \"REST\",\
      \"description\": \"REST API\",\
      \"type\": \"REST_HTTP\",\
      \"protocol\": \"HTTP\",\
      \"version\": \"2.0\",\
      \"endpoint\": {\
        \"addresses\": [\
          {\
            \"host\": \"192.0.2.0\",\
            \"port\": 8080\
          }\
        ]\
      },\
      \"security\": {\
        \"oAuth2Info\": {\
          \"grantTypes\": [\
            \"OAUTH2_CLIENT_CREDENTIALS\"\
          ],\
          \"tokenEndpoint\": \"/mecSerMgmtApi/security/TokenEndPoint\"\
        }\
      }\
    },\
    \"serializer\": \"JSON\"\
  },\
    {\
    \"serInstanceId\": \"ServiceInstance123\",\
    \"serName\": \"ExampleService\",\
    \"serCategory\": {\
      \"href\": \"catItem1\",\
      \"id\": \"id12345\",\
      \"name\": \"RNI\",\
      \"version\": \"version1\"\
    },\
    \"version\": \"ServiceVersion1\",\
    \"state\": \"ACTIVE\",\
    \"transportInfo\": {\
      \"id\": \"TransId12345\",\
      \"name\": \"REST\",\
      \"description\": \"REST API\",\
      \"type\": \"REST_HTTP\",\
      \"protocol\": \"HTTP\",\
      \"version\": \"2.0\",\
      \"endpoint\": {\
        \"addresses\": [\
          {\
            \"host\": \"192.0.2.0\",\
            \"port\": 8080\
          }\
        ]\
      },\
      \"security\": {\
        \"oAuth2Info\": {\
          \"grantTypes\": [\
            \"OAUTH2_CLIENT_CREDENTIALS\"\
          ],\
          \"tokenEndpoint\": \"/mecSerMgmtApi/security/TokenEndPoint\"\
        }\
      }\
    },\
    \"serializer\": \"JSON\"\
  }\
]";

    rapidjson::Document d;
    d.Parse(jsonBody.c_str());

    ASSERT_TRUE(d.IsArray());

    for (rapidjson::Value::ValueIterator itr = d.Begin(); itr!= d.End(); ++itr) {
        list<string> h = {"serInstanceId"};
        rapidjson::Value* v = Util::fatchJsonValue((*itr), h);
        ASSERT_STREQ(v->GetString(), "ServiceInstance123") << "serInstanceId = " << v->GetString() << std::endl;
        cout << "-------------------------serInstanceId: " << v->GetString() << endl;

        h = {"serCategory", "id"};
        v = Util::fatchJsonValue((*itr), h);
        ASSERT_STREQ(v->GetString(), "id12345")<<"serCategory.id= " << v->GetString() << std::endl;
        cout << "-------------------------serCategory.id: " << v->GetString() << endl;

        h = {"transportInfo", "endpoint", "addresses"};
        v = Util::fatchJsonValue((*itr), h);
        for (rapidjson::Document::ValueIterator itr = v->Begin(); itr != v->End(); ++itr) {
            for (rapidjson::Document::ConstMemberIterator i = itr->MemberBegin(); i != itr->MemberEnd(); ++i) {
                if (!std::string("host").compare(i->name.GetString())) {
                    cout << "-------------------------transportInfo.endpoint.addresses.host: " << i->value.GetString() << endl;
                    ASSERT_STREQ(i->value.GetString(), "192.0.2.0")<<"Error!!!!!!" << i->value.GetString() << i->value.GetType() << std::endl;
                }

                if (!string("port").compare(i->name.GetString())) {
                    cout << "-------------------------transportInfo.endpoint.addresses.port: " << i->value.GetInt() << endl;
                    ASSERT_EQ(i->value.GetInt(), 8080)<<"Error!!!!!!" << i->value.GetInt() << i->value.GetType() << endl;
                }
            }
        }
    }
}

TEST(jsontest, writeJson) {
    const std::string JsonBody = "{\
    \"serName\": \"dongyin\",\
    \"transportInfo\": {\
        \"id\": \"TransId12345\",\
        \"name\": \"REST\",\
        \"description\": \"REST API\",\
        \"type\": \"REST_HTTP\",\
        \"protocol\": \"HTTP\",\
        \"version\": \"2.0\",\
        \"endpoint\": { \
            \"addresses\" : [   \
                {   \
                    \"host\" : \"1.1.1.1\", \
                    \"port\" : 8080 \
                }   \
            ] \
        }, \
        \"security\": { \
            \"oAuth2Info\": { \
                \"grantTypes\": [ \
                    \"OAUTH2_CLIENT_CREDENTIALS\" \
                ], \
                \"tokenEndpoint\": \"/mecSerMgmtApi/security/TokenEndPoint\" \
            } \
        },  \
        \"implSpecificInfo\": {} \
        }\
    }";

    Document d;
    d.Parse(JsonBody.c_str());
    auto& a = d.GetAllocator();

    Value* v;
    list<string> h;

    // set serName.
    string s = "dy";
    h = {"serName"};
    v = Util::fatchJsonValue(d, h);
    if (v != nullptr) {
        v->SetString(s.c_str(), a);
    }
    v = Util::fatchJsonValue(d, h);
    ASSERT_STREQ(v->GetString(), "dy") << "Error!!!!!!" << v->GetString() << endl;

    // set addresses
    h = {"transportInfo", "endpoint", "addresses"};
    v = Util::cleanJsonArray(d, h);
    if ((v != nullptr) && (v->IsArray())) {
        string ip("2.2.2.2"); unsigned port = 9090;
        Value addr(rapidjson::kObjectType);
        Value ipv(ip.c_str(), static_cast<SizeType>(ip.size()), a);
        Value portv(port);
        addr.AddMember("host", ipv, a); 
        addr.AddMember("port", portv, a);
        v->PushBack(addr, a);
    }
    v = Util::fatchJsonValue(d, h);
    for (auto itr = v->Begin(); itr != v->End(); ++itr) {
        if (itr->HasMember("host")) {
            ASSERT_STREQ(itr->FindMember("host")->value.GetString(), "2.2.2.2") << "ERROR!!!!!!" << itr->FindMember("host")->value.GetString() << endl;
        }
        if (itr->HasMember("port")) {
            ASSERT_EQ(itr->FindMember("port")->value.GetUint(), 9090) << "ERROR!!!!!!" << itr->FindMember("port")->value.GetUint() << endl;
        }
    }
}

#include "rapidjson/filewritestream.h"
TEST(jsontest, writeJsonFile) {
    /*
    {
    "interface": [{
        "name": "",
        "ver": "",
        "requireFwVer": "",
        "factorys": [{
                "pluginName": ""
            },
            {
                "pluginName": ""
            }
        ]
    },
    {
        "name": "",
        "ver": "",
        "requireFwVer": "",
        "factorys": [{
                "pluginName": ""
            },
            {
                "pluginName": ""
            }
        ]
    }]
    }
    */
    Document d;
    Document::AllocatorType& a = d.GetAllocator();
    d.SetObject();

    Value itfArray(kArrayType);
    Value itf(kObjectType);
    Value factoryArray(kArrayType);
    Value factory(kObjectType);

    itf.AddMember("name", StringRef(string("interface_1").c_str()), a);
    itf.AddMember("ver", StringRef(string("ver_1").c_str()), a);
    itf.AddMember("reqFwVer", Value().SetString(string("require_fw_ver_1").c_str(), a).Move(), a);

    factory.AddMember("pluginName", StringRef(string("plugin_name_1").c_str()), a);
    factoryArray.PushBack(factory,a);

    itf.AddMember("factorys", factoryArray, a);

    itfArray.PushBack(itf, a);

    d.AddMember("itfs", itfArray, a);

    rapidjson::StringBuffer stringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> stringWriter(stringBuffer);
    d.Accept(stringWriter);
    string json = stringBuffer.GetString();

    cout << "====================================" << json << endl;

    FILE* fp = fopen("output.json", "w");
    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    Writer<FileWriteStream> writer(os);
    d.Accept(writer);

    fclose(fp);
}
