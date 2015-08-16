#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <stdarg.h>
#include <iostream>
#include <thread>
#include <mutex>

#define INFO    "Info :"
#define WARNING "Warn :"
#define ERROR   "Error:"
#define DEBUG   "Debug:"


class Log
{
    static std::string _fn;
    static bool _loggingEnabled;
    public:
        static void Init(const std::string& fileName);
        static void SetEnabled(bool);

        static void i (const std::string &text);
        static void e (const std::string &text);
    private:
        static std::mutex mLock;

        static std::string getTimeStamp();
};
#endif
