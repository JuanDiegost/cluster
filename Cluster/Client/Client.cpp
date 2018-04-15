#include "Client.h"

Client::Client(){
    this->serverPort=9000;
    this->serverIP = "localhost";
    this->state = true;
    this->isStorageMachine = false;
}

Client::Client(int port, string ip){
    this->serverPort = port;
    this->serverIP = ip;
    this->state = true;
    this->isStorageMachine = false;
}

void Client::connectServer(){
    clientDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    serverInfo.sin_family = AF_INET;
    cout<<"esto lo imprimo yo: "<<serverIP.c_str()<<endl;
    inet_pton(AF_INET, serverIP.c_str(), &serverInfo.sin_addr);
    serverInfo.sin_port = htons(serverPort);
    int conn = connect(clientDescriptor, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
    if(conn!=-1){
        pthread_t listenThread;
        pthread_create(&listenThread, NULL, listenServer, (void *) this);

        pthread_t writeThread;
        pthread_create(&writeThread, NULL, writeServer, (void *) this);
        while(1);
    }else{
        cout<<"No se pudo conectar con el servidor"<<endl;
    }
}
//aqui es donde escucha por tanto debe hacerse un controlador para los mensajes que llegan dentro de este para que el cleiente sepa que degbe hacer
void * Client::listenServer(void* clientInput){
    Client* client = (Client *) clientInput;
    char serversMessage[60];
    while(1){
        recv(client->getDescriptor(), (void *) &serversMessage, 60, 0);
        cout<<serversMessage<<endl;
    }
}
//en este menu es donde se hace el menu de acciones y se envian los mensajes con lo que sea que el cliente haya escrito
void * Client::writeServer(void* clientInput){
    Client* client = (Client *) clientInput;
    int conected = 1;
//    while(conected){
        cout<<"Seleccione una opcion:"<<endl;
        cout<<"1. Acceder como cliente"<<endl;
        cout<<"2. Acceder como maquina de almacenamiento"<<endl;
        char msg[128];
        int option;
        int i;
        cin>>option;
        fflush(stdin);
        switch (option){
        case 1:
            i = send(client->getDescriptor(), (void *) "1", sizeof(msg), 0);
            break;
        case 2:
            client->setIsStorageMachine(true);
            i = send(client->getDescriptor(), (void *) "2", sizeof(msg), 0);
            break;
        default:
            break;
        }

        sleep(1);
        if(i==-1){
            conected = 0;
            cout<<"se desconecto del servidor"<<endl;
            close(client->getDescriptor());
            exit(EXIT_SUCCESS);
        }
//    }
}

int Client::getDescriptor(void){
    return this->clientDescriptor;
}

void Client::setDescriptor(int descriptor){
    this->clientDescriptor = descriptor;
}

bool Client::getState(void){
    return this->state;
}

void Client::setState(bool state){
    this->state = state;
}

bool Client::getIsStorageMachine(void){
    return this->isStorageMachine;
}

void Client::setIsStorageMachine(bool mode){
    this->isStorageMachine = mode;
}

