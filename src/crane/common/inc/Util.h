/*
 * @Descripttion: 
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 10:31:02
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-04-07 15:28:24
 */
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

#ifndef __CRANE_UTIL_H__
#define __CRANE_UTIL_H__

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <memory>
#include <atomic>

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
public:
    /**
     * @Descripttion: 版本比较
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
private:
    // 将版本的string格式转换成unsigned数组
    static void cvtVer2I(std::string ver, unsigned* iv, unsigned len);
};

class AtomicFlagMutex {
public:
    AtomicFlagMutex() = default;

    AtomicFlagMutex(const AtomicFlagMutex&) = delete;
    AtomicFlagMutex& operator=(const AtomicFlagMutex&) = delete;

    void lock() {
        while(_flag.test_and_set(std::memory_order_acquire));
    }

    void unlock() {
        _flag.clear(std::memory_order_release);
    }

private:
    static constexpr unsigned ATOMIC_FLAG_PADDING_LENGTH 
        = (CACHE_LINE_SIZE_IN_BYTES - sizeof(std::atomic_flag))/sizeof(char);
    atomic_flag     _flag = ATOMIC_FLAG_INIT;
    char            _padding[ATOMIC_FLAG_PADDING_LENGTH];
};

}

#endif
