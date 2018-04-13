/* 
 * File:   main.cpp
 * Author: JuanDiegost
 * 
 * Created on 8 de abril de 2018, 04:53 PM
 */

#include "socketserver.h"


SocketServer *server;

void * serverRun(void *)
{
    try{
        server->run();
    }catch(string ex)
    {
        cout << ex << endl;
    }

   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   server = new SocketServer;
   pthread_t hiloServer;
   pthread_create(&hiloServer,0,serverRun,NULL);
   pthread_detach(hiloServer);

   while (1) {
       string mensaje;
       cin >> mensaje;
       server->setMensaje(mensaje.c_str());
   }

   delete server;
   return 0;
}