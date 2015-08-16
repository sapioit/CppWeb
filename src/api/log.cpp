#include "log.h"
#include <stdarg.h>

bool Log::_loggingEnabled;

void Log::Init(const std::string &fileName)
{
    Log::_fn = fileName;
    Log::_loggingEnabled = false;
}

void Log::SetEnabled(bool state)
{
    Log::_loggingEnabled = state;
}

void Log::i (const std::string &text)
{
    if(_loggingEnabled){
        std::lock_guard<std::mutex> lock(mLock);
        std::fstream stream(_fn, std::ios::out | std::ios::app);
        if(stream.is_open() == false) return;
        stream << getTimeStamp () << " Info  : " << text << std::endl;
    }
}

void Log::e (const std::string &text)
{   if(_loggingEnabled) {
        std::lock_guard<std::mutex> lock(mLock);
        std::fstream stream(_fn, std::ios::out | std::ios::app);
        if(stream.is_open() == false) return;
        stream << getTimeStamp () << " Info  : " << text << std::endl;
    }
}


std::string Log::getTimeStamp ()
{
    std::string text;
    text.resize(100);
    time_t t = time (0);
    struct tm today   = *gmtime(&t);

    auto len = strftime(&text.front(), text.size(), "%a, %d %b %Y %H:%M:%S %Z", &today);
    text.resize(len);
    return text;
}
