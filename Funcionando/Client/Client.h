#ifndef CLIENT_H
#define CLIENT_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <cstdio>
#include <stdlib.h>
#include <pthread.h>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <fstream>

using namespace std;

class Client
{
    public:
        //metodos:
        Client();
        Client(int, string);

        void connectServer(void);
        static void * listenServer(void *);
        static void * writeServer(void *);

        int getDescriptor(void);
        void setDescriptor(int);

        bool getState(void);
        void setState(bool);

        bool getIsStorageMachine(void);
        void setIsStorageMachine(bool);

        static inline void *sendFile(void * clientInput, const char *path);
        static inline void *receiveFile(void* infoClient, const char *path);
        static int getFileSize(std::string);
        static int getCurrentMachineStorage();

    private:
        //atributos:
        int clientDescriptor;
        struct sockaddr_in serverInfo;
        int serverPort;
        string serverIP;
        bool state;
        bool isStorageMachine;
};

#endif // CLIENT_H
