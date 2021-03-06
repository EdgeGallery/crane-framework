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

#ifndef __CRANELOG_H__
#define __CRANELOG_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

#include <chrono>
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>
#include <mutex>

#include "craneMacro.h"

using namespace std;

namespace NS_CRANE {
    #define gettid() syscall(__NR_gettid)
    
    #define FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
    #define LOG_DEBUG(...)    crane::Log::writeLog(crane::Log::Level::DEBUG, (unsigned char*)(FILENAME(__FILE__)), (unsigned char*)(__FUNCTION__), (int)(__LINE__), __VA_ARGS__)
    #define LOG_INFO(...)     crane::Log::writeLog(crane::Log::Level::INFO, (unsigned char*)(FILENAME(__FILE__)), (unsigned char*)(__FUNCTION__), (int)(__LINE__), __VA_ARGS__)
    #define LOG_WARNING(...)  crane::Log::writeLog(crane::Log::Level::WARNING, (unsigned char*)(FILENAME(__FILE__)), (unsigned char*)(__FUNCTION__), (int)(__LINE__), __VA_ARGS__)
    #define LOG_ERROR(...)    crane::Log::writeLog(crane::Log::Level::ERROR, (unsigned char*)(FILENAME(__FILE__)), (unsigned char*)(__FUNCTION__), (int)(__LINE__), __VA_ARGS__)
    
    #define LOG_ENTER() LOG_INFO("enter")
    #define LOG_EXIT()  LOG_INFO("exit")
    
    // 单个日志文件最大存储
    #define MAX_SIZE 2*1024*1024  // 2M
    
    using namespace std;
    
    class Log
    {
    public:
        enum class Level
        {
            NONE,
            ERROR,     // error
            WARNING,   // warning
            INFO,      // info
            DEBUG,     // debug
        };

        enum class Target
        {
            NONE      = 0x00,
            CONSOLE   = 0x01,
            FILE      = 0x10
        };

        void init(Level level, Target target);

        void uninit();

        int createFile();

        static Log* getInstance();

        // Log级别
        Level getLevel();
        void setLevel(Level level);

        // Log输出位置
        Target getTarget();
        void setTarget(Target target);

        // 打log
        static int writeLog(
            Level loglevel,         // Log级别
            unsigned char* fileName,   // 函数所在文件名
            unsigned char* function,   // 函数名
            int lineNumber,            // 行号
            const char* format,        // 格式化
            ...);                      // 变量

        // 输出log
        static void outputToTarget();
        
        Level cvtLevel(string &level);

        Target cvtTarget(string &target);

    private:
        Log();

        virtual ~Log();
        
        static Log* instance;

        // 互斥锁		
        static mutex log_write_mutex;
        // 临界区
        static mutex log_create_mutex; 

        // 存储log的buffer
        static string logBuffer;
        // 已写的log长度
        static string::size_type writtenSize;

        // Log级别
        Level level;

        // Log输出位置
        Target target;

        // Handle
        static int mFileHandle;

        array<pair<string, Level>, 5> _LevelEnumArray;
        void _initializeLevleArray()
        {
            _LevelEnumArray[0] = std::make_pair("NONE",    Level::NONE);
            _LevelEnumArray[1] = std::make_pair("ERROR",   Level::ERROR);
            _LevelEnumArray[2] = std::make_pair("WARNING", Level::WARNING);
            _LevelEnumArray[3] = std::make_pair("INFO",    Level::INFO);
            _LevelEnumArray[4] = std::make_pair("DEBUG",   Level::DEBUG);
        }

        array<pair<string, Target>, 3> _TargetEnumArray;
        void _initializeTargetArray() {
            _TargetEnumArray[0] = std::make_pair("NONE",    Target::NONE);
            _TargetEnumArray[1] = std::make_pair("CONSOLE", Target::CONSOLE);
            _TargetEnumArray[2] = std::make_pair("FILE",    Target::FILE);
        }
    };
    }

    #endif
