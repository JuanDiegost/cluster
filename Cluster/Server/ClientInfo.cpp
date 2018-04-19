#include "ClientInfo.h"

ClientInfo::ClientInfo(int descriptor, struct sockaddr_in info){
    this->clientDescriptor = descriptor;
    this->clientInfo = info;
    this->state = true;
}

int ClientInfo::getId(){
    return this->id;
}

void ClientInfo::setId(int id){
    this->id = id;
}

int ClientInfo::getClientDescriptor(){
    return this->clientDescriptor;
}

struct sockaddr_in ClientInfo::getClientInfo(){
    return this->clientInfo;
}

bool ClientInfo::getState(){
    return this->state;
}

void ClientInfo::setState(bool state){
    this->state = state;
}

std::string ClientInfo::getType(){
    return this->type;
}

void ClientInfo::setType(std::string type){
    this->type=type;
}
