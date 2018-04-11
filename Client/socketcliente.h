/* 
 * File:   socketcliente.h
 * Author: JuanDiegost
 *
 * Created on 8 de abril de 2018, 05:06 PM
 */

#ifndef SOCKETCLIENTE_H
#define SOCKETCLIENTE_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

class SocketCliente
{
public:
    SocketCliente();
    bool connectar();
    void setMensaje(const char *msn);
private:
    int descriptor;
    sockaddr_in info;
    static void * controlador(void *obj);
};

#endif // SOCKETCLIENTE_H