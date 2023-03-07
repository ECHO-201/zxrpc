#include "Load_balance.h"
#include <random>

LoadBalance::LoadBalance()
{
}

LoadBalance::~LoadBalance()
{
}

std::string LoadBalance::ramdomStrategy(){
    std::uniform_int_distribution<int> rd(0, services.size() - 1);
    std::random_device rd1;
    return services[rd(rd1)];
}

void LoadBalance::set_service(const std::string &service_name){
    services.push_back(service_name);
}