#pragma once
#include <string>

struct user{
    std::string id;
    std::string name;
    std::string passwd;
};

struct log_data{
    std::string id;
    std::string record;
    std::string ip;
    std::string port;
};
