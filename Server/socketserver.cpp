#include "socketserver.h"

#define LENGTH 1024000

SocketServer::SocketServer() {
}

bool SocketServer::crear_Socket() {
    descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (descriptor < 0)
        return false;
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = INADDR_ANY;
    info.sin_port = htons(4050);
    memset(&info.sin_zero, 0, sizeof (info.sin_zero));
    return true;
}

bool SocketServer::ligar_kernel() {
    if ((bind(descriptor, (sockaddr *) & info, (socklen_t)sizeof (info))) < 0)
        return false;
    listen(descriptor, 5);
    return true;
}

void SocketServer::run() {
    if (!crear_Socket())
        throw string("Error al crear el socket");
    if (!ligar_kernel())
        throw string("Error al  ligar kernel");
    while (true) {
        cout << "Esperando nuevo cliente" << endl;
        dataSocket data;
        socklen_t tam = sizeof (data.info);
        data.descriptor = accept(descriptor, (sockaddr*) & data.info, &tam);
        if (data.descriptor < 0)
            cout << "Error al acceptar al cliente" << endl;
        else {
            cout << "Nuevo cliente" << endl;
            clientes.push_back(data.descriptor);
            pthread_t hilo;
            pthread_create(&hilo, 0, SocketServer::controladorCliente, (void *) &data);
            pthread_detach(hilo);
        }
    }
    close(descriptor);
}

void * SocketServer::controladorCliente(void *obj) {
    dataSocket *data = (dataSocket*) obj;
    while (true) {
#define BLOCK 1024
        string mensaje;
        char buffer[256] = {0};
        while (1) {
            int bytes = recv(data->descriptor, (void *) &buffer, 256, 0);
            if (bytes <= 0) {
                close(data->descriptor);
                pthread_exit(NULL);
            }
            if (bytes < 256) {
                break;
            }
        }
        cout << mensaje << endl;
        switch (buffer[0]) {
            case '1':
                send(data->descriptor, "accepted", sizeof "accepted", 0);
                int size, ofs, nbytes = 0, block = BLOCK;
                char pbuf[BLOCK];
                std::string path = "/root/Escritorio/Server/test.png";
                std::ofstream os(path, std::ios::binary);
                cout << "recibir" << endl;
                recv(data->descriptor, reinterpret_cast<char*> (&size), sizeof size, 0);
                cout << "DEscriptor" << endl;

                for (ofs = 0; block == BLOCK; ofs += BLOCK) {
                    if (size - ofs < BLOCK) block = size - ofs;
                    nbytes += recv(data->descriptor, pbuf, block, 0);
                    os.write(pbuf, block);
                }
                cout << "salie" << endl;

                os.close();
                close(data->descriptor);
                pthread_exit(NULL);
                break;
        }


    }
}

void SocketServer::setMensaje(const char *msn) {
    for (unsigned int i = 0; i < clientes.size(); i++) {
        cout << "bytes enviados " << send(clientes[i], msn, strlen(msn), 0) << endl;
    }
}
