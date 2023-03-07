#pragma once

#include <string>
#include <vector>

class LoadBalance
{   
public:
    LoadBalance();
    ~LoadBalance();

    std::string ramdomStrategy();

    void set_service(const std::string &service_name);

private:
    std::vector<std::string> services;
};


