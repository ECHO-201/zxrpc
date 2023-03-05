#pragma once
#include "lockqueue.h"
#include <string>

#include "MyDB.h"

// 定义宏 LOG_XXXX("xxx %d %s", 20, "xxxx")
#define LOG_INFO(logmsgformat, ...) \
    do \
    {  \
        Logger &Logger = Logger::GetInstance(); \
        Logger.SetLogLevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        Logger.Log(c); \
    } while(0) \

#define LOG_ERROR(logmsgformat, ...) \
    do \
    {  \
        Logger &Logger = Logger::GetInstance(); \
        Logger.SetLogLevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        Logger.Log(c); \
    } while(0) \


#define LOG_DEBUG(logmsgformat, ...) \
    do \
    {  \
        Logger &Logger = Logger::GetInstance(); \
        Logger.SetLogLevel(DEBUG); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        Logger.Log(c); \
    } while(0) \

#define LOG_WARN(logmsgformat, ...) \
    do \
    {  \
        Logger &Logger = Logger::GetInstance(); \
        Logger.SetLogLevel(WARN); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        Logger.Log(c); \
    } while(0) \

#define LOG_FATAL(logmsgformat, ...) \
    do \
    {  \
        Logger &Logger = Logger::GetInstance(); \
        Logger.SetLogLevel(FATAL); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        Logger.Log(c); \
    } while(0) \

// 定义日志级别
enum LogLevel
{
    DEBUG,
    INFO,  // 普通信息
    WARN,
    ERROR, // 错误信息
    FATAL,
    LEVEL_COUNT
};

namespace watcher {
namespace tools {

class Logger
{
public:

    /**
     * @brief 获取日志的单例
     */
    static Logger& GetInstance();

    /**
     * @brief 设置日志级别
     */ 
    void SetLogLevel(LogLevel level);

    /**
     * @brief 写日志
     */ 
    void Log(std::string msg);
private:
    int m_loglevel; // 记录日志级别
    LockQueue<std::string> m_lckQue; // 日志缓冲队列
    static const char* str_level[LEVEL_COUNT];

    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

    std::string m_ip;
    std::string m_port;
    MyDB *db;
};

}}