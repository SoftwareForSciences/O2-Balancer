#include "O2/Balancer/Devices/Connection.h"
#include "O2/Balancer/Devices/AbstractDevice.h"
#include "O2/Balancer/Exceptions/UnimplementedException.h"
#include "O2/Balancer/Utilities/DeviceSetting.h"
#include <chrono>
#include <thread>
using namespace O2::Balancer;

Connection::Connection(const std::string& name, AbstractDevice* device){
    this->name = name;
    this->device = device;
    this->device->fChannels.insert(
        std::pair<std::string, std::vector<FairMQChannel>>(name, std::vector<FairMQChannel>()));
}

void Connection::updateAllRateLogging(const int& logg){
     for(auto& i : this->device->fChannels.at(name)){
        i.UpdateRateLogging(logg);
    }
}

void Connection::updateAllReceiveBuffer(const int& buffer){
    for(auto& i : this->device->fChannels.at(name)){
        i.UpdateRcvBufSize(buffer);
    }
}

void Connection::updateAllSendBuffer(const int& buffer){
    for(auto& i : this->device->fChannels.at(name)){
        i.UpdateSndBufSize(buffer);
    }
}

void Connection::updateAllSendKernelSize(const int& size){
    for(auto& i : this->device->fChannels.at(name)){
        i.UpdateSndKernelSize(size);
    }
}

void Connection::updateAllReceiveKernelSize(const int& size){
    for(auto& i : this->device->fChannels.at(name)){
        i.UpdateRcvKernelSize(size);
    }
}

std::string Connection::typeToString(ConnectionType type) const{
    switch(type){
        case ConnectionType::Publish:
        return "pub";
        case ConnectionType::Pull:
        return "pull";
        case ConnectionType::Subscribe:
        return "sub";
        case ConnectionType::Push:
        return "push";
        default:
        throw Exceptions::UnimplementedException("typeToString doesn't implement this case");
    }
}

std::string Connection::methodToString(ConnectionMethod method) const{
    switch(method){
        case ConnectionMethod::Bind:
        return "bind";
        case ConnectionMethod::Connect:
        return "connect";
        default:
        throw Exceptions::UnimplementedException("MethodToString doesn't implement this case");
    }
}

std::shared_ptr<DeviceSetting> Connection::addInputChannel(ConnectionType type, ConnectionMethod method,const std::string& ip, int port){
    this->device->fChannels.at(name).push_back( FairMQChannel(
        this->typeToString(type),
        this->methodToString(method),
        "tcp://" + std::string(ip) + ":" + std::to_string(port)
    ));
    return std::shared_ptr<DeviceSetting>(new DeviceSetting(port,ip));
}

std::shared_ptr<DeviceSetting> Connection::addOutputChannel(ConnectionType type, ConnectionMethod method, const std::string& ip, int port){
    //Stop what you are doing, and wait until it's registered in ZooKeeper. In case of failure try again later
    while(!device->addHandle(this->name, O2::Balancer::DeviceSetting(port,ip))){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return this->addInputChannel(type,method,ip,port);
   
}


void Connection::updateChannel(std::shared_ptr<DeviceSetting> oldChannel,std::shared_ptr<DeviceSetting> newChannel){
    for(auto& i : this->device->fChannels.at(name)){
        if(i.GetAddress() == "tcp://" + oldChannel->ip + ":" + std::to_string(oldChannel->port)){
            i.UpdateAddress("tcp://" + newChannel->ip + ":" + std::to_string(newChannel->port));
            LOG(INFO) << "Updated";
            return;
        }
    }
}


void Connection::updateConnection(std::shared_ptr<ClusterManager> clusterManager){
    //virtual
}

std::string Connection::getName() const{
    return this->name;
}