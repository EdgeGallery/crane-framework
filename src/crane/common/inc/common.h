/*
 * @Descripttion: Contain the all base H files which would used by crane framework and system plugins.
 * @Version: 1.0
 * @Author: dongyin@huawei.com
 * @Date: 2021-02-23 11:21:54
 * @LastEditors: dongyin@huawei.com
 * @LastEditTime: 2021-02-24 11:19:18
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

#ifndef __COMMON_H__
#define __COMMON_H__

// C H files
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>

#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <assert.h>

// C++ H files
#include <exception>
#include <functional>

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <vector>
#include <queue>
#include <list>
#include <map>

#include <memory>
#include <unordered_map>

#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

// boost  H files TODO: boost should be removed, because it is so big.
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

// Rapidjson files
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>
#include <rapidjson/filereadstream.h>

#endif