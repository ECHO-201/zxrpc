#pragma once

#include <string>
using namespace std;

namespace zxrpc {
namespace tools {

class System
{
public:
    System();
    ~System();

    void init();
    string get_root_path();

private:
    // 创建dump文件，用来之后DEBUG
    void core_dump();

private:
    string m_root_path;
};

}}
