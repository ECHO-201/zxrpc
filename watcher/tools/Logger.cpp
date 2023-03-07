#include "Logger.h"
#include <time.h>
#include <iostream>
#include "config_file.h"

using namespace watcher::tools;

const char* Logger::str_level[LEVEL_COUNT] = 
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

Logger& Logger::GetInstance()
{
    static Logger Logger;
    return Logger;
}

Logger::Logger()
{
    std::thread writeLogTask([&](){
        for (;;)
        {
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name, "log/%d-%d-%d-log.txt", nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);

            FILE *pf = fopen(file_name, "a+");
            if (pf == nullptr)
            {
                std::cout << "Logger file : " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = m_lckQue.Pop();
            std::string log_level = str_level[m_loglevel];
            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d=>[%s] ", 
                    nowtm->tm_hour, 
                    nowtm->tm_min, 
                    nowtm->tm_sec,
                    log_level.c_str());
            msg.insert(0, time_buf);
            msg.append("\n");
            fputs(msg.c_str(), pf);
            fclose(pf);
        }
    });

    writeLogTask.detach();
}

Logger::~Logger(){
    delete db;
    db = NULL;
}


void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}

void Logger::Log(std::string msg)
{
    m_lckQue.Push(msg);
}