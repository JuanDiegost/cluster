/*
 * File:   main.cpp
 * Author: JuanDiegost
 *
 * Created on 8 de abril de 2018, 04:53 PM
 */

#include "socketserver.h"
using namespace std;


void * serverRun(void * serve)
{
    SocketServer * serv = (SocketServer *) serve;
    try{
        serv->run();
    }catch(string ex)
    {
        cout << ex << endl;
    }

   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   SocketServer *server = new SocketServer();
   pthread_t hiloServer;
   pthread_create(&hiloServer,0,serverRun,(void*)server);
   pthread_detach(hiloServer);
   while (1) {
       string mensaje;
       cin >> mensaje;
       server->setMensaje(mensaje.c_str());
   }
   delete server;
   return 0;
}
