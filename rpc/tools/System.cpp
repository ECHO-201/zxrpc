#include "System.h"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>
using namespace zxrpc::tools;
#include "Logger.h"
#include "config_file.h"
#include "Singleton.h"
#include "Workflow.h"

using namespace zxrpc::work;


System::System()
{
}

System::~System()
{
}

void System::init()
{
    core_dump();

    m_root_path = get_root_path(); 
    const string logDir = m_root_path + "/log";
    //init Logging
    DIR *dir = opendir(logDir.c_str());
    if(dir == NULL){
        mkdir(logDir.c_str(), 0755);
    }else{
        //目录文件作为一种文件，再打开之前要先关闭，否则会由于文件的进程打开文件过多导致不能打开新的文件
        closedir(dir); 
    }

    // init workflow
    Workflow * workflow = Singleton<Workflow>::instance();
    workflow->load(get_root_path() + "/config/workflow.xml");
}

void System::core_dump()
{
    // core dump
    struct rlimit x;
    int ret = getrlimit(RLIMIT_CORE, &x);
    x.rlim_cur = x.rlim_max;
    ret = setrlimit(RLIMIT_CORE, &x);

    ret = getrlimit(RLIMIT_DATA, &x);
    x.rlim_cur = 768000000;
    ret = setrlimit(RLIMIT_DATA, &x);
}

std::string System::get_root_path()
{
    if (m_root_path != "")
    {
        return m_root_path;
    }
    char path[1024];
    memset(path, 0, 1024);
    int cnt = readlink("/proc/self/exe", path, 1024);
    if (cnt < 0 || cnt >= 1024)
    {
        return "";
    }
    for (int i = cnt; i >= 0; --i)
    {
        if (path[i] == '/')
        {
            path[i] = '\0';
            break;
        }
    }
    return string(path);
}
