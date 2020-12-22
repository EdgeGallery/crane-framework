#include "Log.h"

namespace NS_CRANE {

    Log*                Log::instance         = nullptr;
    string              Log::logBuffer   = "";
    int                 Log::mFileHandle = -1;
    mutex     			Log::log_create_mutex; 
    mutex			    Log::log_write_mutex;
    string::size_type   Log::writtenSize = 0;
    
    Log::Log() {
    	init(Level::DEBUG, Target::FILE);
		_initializeLevleArray();
		_initializeTargetArray();
    } 

    Log::~Log() {
        uninit();
    }
    
    void Log::init(Level level, Target target)
    {
    	setLevel(level);
    	setTarget(target);
    	createFile();
    }
    
    void Log::uninit()
    {
    	if (-1 != mFileHandle)
    	{
            close(mFileHandle);
    	}
    }
    
    Log* Log::getInstance()
    {
    	if (nullptr == instance)
    	{
			lock_guard<mutex> guard(log_create_mutex);
    		if (nullptr == instance)
    		{
    			instance = new Log();
    		}
            //pthread_mutex_unlock(&log_mutex);
    	}
    	return instance;
    }
    
    Log::Level Log::getLevel()
    {
    	return this->level;
    }
    
    void Log::setLevel(Level level)
    {
    	this->level = level;
    }
    
    Log::Target Log::getTarget()
    {
    	return this->target;
    }
    
    void Log::setTarget(Target target)
    {
    	this->target = target;
    }

	Log::Level Log::cvtLevel(string &level) {
		for (auto l : _LevelEnumArray) {
			if (l.first == level) {
				return l.second;
			}
		}
		return _LevelEnumArray[0].second;
	}

    Log::Target Log::cvtTarget(string &target) {
		for (auto t : _TargetEnumArray) {
			if (t.first == target) {
				return t.second;
			}
		}
		return _TargetEnumArray[0].second;
	}
    
    int Log::createFile()
    {
        mFileHandle = open("/root/crane.log", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU|S_IRGRP|S_IWGRP);
        if (mFileHandle <= 0) {
            return -1;
        } else {
    	    return 0;
        }
    }
    
    static int printfToBuffer(char* buffer, int size, const char* format, ...){
        //unused(size);

    	va_list ap;
    	va_start(ap, format);
    	int ret = vsnprintf(buffer, size, format, ap);
    	va_end(ap);
    	return ret;
    }
    
    static string getTime() {
    		auto now = chrono::system_clock::now();
    		auto m = now.time_since_epoch();
    		auto diff = std::chrono::duration_cast<std::chrono::microseconds>(m).count(); //当前时间显示到微秒
    		auto const ms = diff % 1000000;
    
    		stringstream ss;
    		time_t t = chrono::system_clock::to_time_t(now);
    		ss << put_time(localtime(&t), "[%Y-%m-%d %H.%M.%S") << "." << ms <<"]";
    		return ss.str();    
    }
    
    
    int Log::writeLog(
    	Level level,         // Log级别
    	unsigned char* fileName,   // 函数所在文件名
    	unsigned char* function,   // 函数名
    	int lineNumber,            // 行号
    	const char* format,              // 格式化
    	...)
    {
    	int ret = 0;
    
        if (level > Log::getInstance()->getLevel()) { return -1; }

    	// LOG级别
    	const char* cLogLevel;
    	if (level == Level::DEBUG){
    		cLogLevel = "DEBUG";
    	} else if (level == Level::INFO){
    		cLogLevel = "INFO";
    	} else if (level == Level::WARNING){
    		cLogLevel = "WARNING";
    	} else if (level == Level::ERROR){
    		cLogLevel = "ERROR";
    	} else {
            cLogLevel = "ERROR";
        }        

		lock_guard<mutex> guard(log_write_mutex);

    	logBuffer += getTime();
    
    	// [进程号][线程号][Log级别][文件名][函数名:行号]
    	char locInfo[256];
    	const char* format2 = "[PID:%4d][TID:%4d][%s][%-s][%s:%4d]";
    	ret = printfToBuffer(locInfo, 256, format2,
            getpid(),
            gettid(),
    		cLogLevel,
    		fileName,
    		function,
    		lineNumber);
    	logBuffer += string(locInfo);	
    
    	// 日志正文
    	char logInfo2[2048];
    	va_list ap;
    	va_start(ap, format);
    	ret = vsnprintf(logInfo2, 2048, format, ap);
    	va_end(ap);
    
    	logBuffer += string(logInfo2);
    	logBuffer += string("\n");
    	writtenSize += logBuffer.length();
    
    	outputToTarget();
        
    
    	return ret;
    }
    
    void Log::outputToTarget()
    {
        int ret = 0;
        ssize_t size = 0;
    	if (static_cast<int>(Log::getInstance()->getTarget()) & (int)Target::FILE)
    	{
            lseek(mFileHandle, 0, SEEK_END);
            size = write(mFileHandle, logBuffer.c_str(), logBuffer.length());
            ret = syncfs(mFileHandle);
			ret = ret; size = size;
    	}
    	if (static_cast<int>(Log::getInstance()->getTarget()) & (int)Target::CONSOLE)
    	{
    		printf("%s", logBuffer.c_str());
            fflush(stdout);
    	}
    
    	logBuffer.clear();
    }

}