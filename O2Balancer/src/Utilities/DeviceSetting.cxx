#include "O2/Balancer/Utilities/DeviceSetting.h"

using namespace O2::Balancer;

DeviceSetting::DeviceSetting(int port, std::string ip){
    this->port = port;
    this->ip = ip;
}