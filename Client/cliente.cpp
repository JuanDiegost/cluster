/* 
 * File:   main.cpp
 * Author: JuanDiegost
 *
 * Created on 6 de abril de 2018, 09:47 AM
 */


#include "socketcliente.h"

using namespace std;

SocketCliente *conexion;

void * clienteRun(void *) {
   if (!conexion->connectar())
        cout << "Error al conectar con el servidor" << endl;
    pthread_exit(NULL);
}

int main() {
    conexion = new SocketCliente;
    pthread_t hiloCliente;
    pthread_create(&hiloCliente, 0, clienteRun, NULL);
    pthread_detach(hiloCliente);

    while (1) {
        string mensaje;
        cin >> mensaje;
        conexion->setMensaje(mensaje.c_str());
    }

    delete conexion;
    return 0;
}
