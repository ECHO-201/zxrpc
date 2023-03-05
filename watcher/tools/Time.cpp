#include "Time.h"
#include <time.h>
#include <stdio.h>

using namespace watcher::tools;

Time::Time()
{
}

Time::~Time()
{
}

std::string Time::getTime(){
    time_t now = time(nullptr);
    tm *nowtm = localtime(&now);
	char time[128];
	sprintf(time, "%d-%d-%d-%d-%d", nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday,
            nowtm->tm_hour, nowtm->tm_sec);

    return time;
}