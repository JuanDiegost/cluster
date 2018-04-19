#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <cstdio>
#include <string.h>
#include <fstream>
#include "ClientInfo.h"


using namespace std;

class Server{

private:
    int descriptorServer;
    int idBind;
    int port;
    struct sockaddr_in serverInfo;
    bool state;
    int cont;

public:
   static vector<ClientInfo *> clientsDescriptors;

    //constructores

    Server(void);
    Server(int);

    //Metodos

    void initializeServer(void);
    void acceptClients(void);
    void closeServer(void);
    static void * startServer(void *);
    void executeServer(void);
    static inline void *receiveFile(void* infoClient, const char *path);

    //set

    void setDescriptorServer(int);
    void setIdBind(int);
    void setPort(int);
    void setServerInfo(struct sockaddr_in);

    //get

    int getDescriptorServer(void);
    int getIdBind(void);
    int getPort(void);
    struct sockaddr_in getServerInfo(void);
    vector<ClientInfo*> getClients(void);

};



#endif // SERVERNEW_H_INCLUDED
