/* 
 * File:   socketcliente.cpp
 * Author: JuanDiegost
 * 
 * Created on 8 de abril de 2018, 05:06 PM
 */

#include "socketcliente.h"

SocketCliente::SocketCliente() {
}

bool SocketCliente::connectar() {
    descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (descriptor < 0)
        return false;
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = ntohs(4050);
    inet_pton(AF_INET, "127.0.0.1", &info.sin_addr);
    bzero(&(info.sin_zero), 8);

    if ((connect(descriptor, (sockaddr*) & info, (socklen_t)sizeof (info))) < 0)
        return false;

    pthread_t hilo;
    pthread_create(&hilo, 0, SocketCliente::controlador, (void *) this);
    pthread_detach(hilo);
    return true;
}

void * SocketCliente::controlador(void *obj) {
    SocketCliente *padre = (SocketCliente*) obj;
    while (true) {
        string mensaje;
        while (1) {
            char buffer[256] = {0};
            int bytes = recv(padre->descriptor, buffer, 256, 0);
            mensaje.append(buffer, bytes);
            if (bytes <= 0) {
                close(padre->descriptor);
                pthread_exit(NULL);
            }
            if (bytes < 256) {
                break;
            }
        }
        cout << mensaje << endl;
    }

    close(padre->descriptor);
    pthread_exit(NULL);
}

void SocketCliente::setMensaje(const char *msn, int fs, long size) {
    cout << "bytes enviados " << send(descriptor, msn, fs, size) << endl;
}

int SocketCliente::getSocket() {
    return descriptor;
}
