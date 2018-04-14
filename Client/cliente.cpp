/* 
 * File:   main.cpp
 * Author: JuanDiegost
 *
 * Created on 6 de abril de 2018, 09:47 AM
 */


#include "socketcliente.h"
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "fstream"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string.h>
#include <asm/byteorder.h>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <dirent.h>



#define LENGTH 1024000


using namespace std;

SocketCliente *conexion;

void * clienteRun(void *) {
    if (!conexion->connectar())
        cout << "Error al conectar con el servidor" << endl;
    pthread_exit(NULL);
}

void sendFile(const char *path) {
#define BLOCK 1024
    int size, ofs, nbytes = 0, block = BLOCK;
    char pbuf[BLOCK];
    std::ifstream is;
    is.open(path, std::ios::binary);
    is.seekg(0, std::ios::end);
    size = is.tellg();
    is.seekg(0, std::ios::beg);
    send(conexion->getSocket(), reinterpret_cast<char*> (&size), sizeof size, 0);
    for (ofs = 0; block == BLOCK; ofs += BLOCK) {
        if (size - ofs < BLOCK) block = size - ofs;
        is.read(pbuf, block);
        nbytes += send(conexion->getSocket(), pbuf, block, 0);
    }
    is.close();
    cout<<"salio";
}

int main() {
    conexion = new SocketCliente;
    pthread_t hiloCliente;
    pthread_create(&hiloCliente, 0, clienteRun, NULL);
    pthread_detach(hiloCliente);
    string ruta;
    char * mensaje;
    bool salir = false;
    char tecla;

    do {
        system("clear");
        cin.clear();
        cout << "Opciones" << endl;
        cout << "-----------" << endl << endl;
        cout << "\t1 .- Subir recurso" << endl;
        cout << "\t2 .- Solicitar listado de recursos por maquina" << endl;
        cout << "\t0 .- Salir" << endl << endl;
        cout << "Elije una opcion: ";
        fflush(stdin);
        cin >> tecla;
        string path = "/home/srfeudal/Documentos/c++_projects/cluster-master/Client/41.png";
        string mensaje = "";
        //tecla = '1';
        cout << "Path: ";
        switch (tecla) {
            case '1':
                //system("cls");
                //cout << "Has elejido dubir un recurso.\n por favor indica su ruta\n";
                //cin >> ruta;
                //leer("2.jpg");
                cout << "send: ";
                char bufferAction[1];
                bufferAction[0] = '1';  
                send(conexion->getSocket(), (void *) &bufferAction,  sizeof bufferAction, 0);//envia mensaje de peticion enviar archivo
                mensaje = "";
                while (1) {
                    char buffer[256];
                    int bytes;
                    bytes = recv(conexion->getSocket(), buffer, 256, 0);
                    mensaje.append(buffer, bytes);
                    if (bytes <= 0) {
                        close(conexion->getSocket());
                        pthread_exit(NULL);
                    }
                    if (bytes < 256) {
                        break;
                    }
                }
                cout << mensaje << endl;
                mensaje = "";   
                
                sendFile(path.data());
                
                cout << "sender: ";
                
                break;
            case '2':
                system("cls");
                cout << "Lista.\n";
                break;
            case '0':
                salir = true;
                break;
            default:
                system("cls");
                cout << "Opcion no valida.\a\n";
                break;
        }
    } while (!salir);

    delete conexion;
    return 0;
}
