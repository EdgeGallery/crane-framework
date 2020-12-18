/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2020-06-18 10:59:01
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2020-09-24 15:12:00
 */ 
#ifndef __CRANE_UTIL_H__
#define __CRANE_UTIL_H__

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <memory>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>
#include <rapidjson/filereadstream.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "craneMacro.h"
#include "Log.h"

using namespace std;
using namespace rapidjson;

namespace NS_CRANE 
{
    template <typename... Args> inline void unused(Args&& ...) {}

    template <typename... Args> inline void server_process_func_args_check() {
        static_assert(sizeof...(Args) == 2, "Function should has 2 parameters.");
    }

    enum class HttpMethod {
        POST = 0,
        PUT,
        GET,
        DELETE
    };

   class Util {
      private:
          /**
           * @Descripttion: 将版本的string格式转换成unsigned数组
           * @Param: 
           * @Return: 
           */        
          static void cvtVer2I(std::string ver, unsigned* iv, unsigned len);

      public:
          /**
           * @Descripttion: 版本比较
           * @Param: 
           * @Return: CRANE_HIGHER:   ver1版本高于ver2
           *          CRANE_LOWER:    ver1版本低于ver2   
           *          CRANE_EQUAL:    ver1和ver2的版本相等
           */        
        static int compareVersion(const std::string ver1, const std::string ver2);

        static unsigned parseJsonFile(const string& absoluteFilename, Document& d);

        static Value* fatchJsonValue(Document& document, const list<string>& hierarchy);

        static Value* fatchJsonValue(Value& value, const list<string>& hierarchy);

        static Value* cleanJsonArray(Document& document, const list<string>& hierarchy);

        static string Json2String(Document& document);

        static void printJson(Document& document);

        static string writeJsonString(const rapidjson::Value& value, const std::string& name, const std::string& value1);

        static string jsonReadString(rapidjson::Document d, const std::string name, const rapidjson::Type type, const unsigned level);

        static string uuid(void);

        static unsigned char* hexstr2uint8(string&, size_t* len);

   }; 
} // namespace 

#endif