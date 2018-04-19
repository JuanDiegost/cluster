#ifndef CLIENTINFO_H
#define CLIENTINFO_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <cstdio>
#include <string.h>
//clase que contiene la informacion de cada cliente es un similar a la clase conexion,
//el server tiene una lista <vector> de esta clase en donde se guardan los clientes
class ClientInfo
{
    public:
        ClientInfo(int, struct sockaddr_in);
        int getId(void);
        void setId(int);
        int getClientDescriptor(void);
        struct sockaddr_in getClientInfo();
        bool getState(void);
        void setState(bool);
        std::string getType();
        void setType(std::string type);
    private:
        int clientDescriptor;
        struct sockaddr_in clientInfo;
        bool state;
        int id;
        std::string type;
};

#endif // CLIENTINFO_H
