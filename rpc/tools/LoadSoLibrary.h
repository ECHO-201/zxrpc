#pragma once

#include <string>
#include <dlfcn.h>
#include "Logger.h"

namespace zxrpc {
namespace tools {

class LoadSoLibrary{
public:
    LoadSoLibrary();
    ~LoadSoLibrary();

    template<typename T>
    T run(std::string &path, std::string &func_name);
private:
    void* handle;
};    

template <typename T>
T LoadSoLibrary::run(std::string &path, std::string &func_name){
    handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        LOG_DEBUG("dlerror is %s", dlerror());
        exit(EXIT_FAILURE);
    }
    T func = reinterpret_cast<T>(
        dlsym(handle, func_name.c_str()));
    return func;
};

}
}