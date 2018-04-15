#include "Server.h"

struct parameter {
    ClientInfo * clientInfo;
    vector<ClientInfo*> clients;
};

Server::Server(){
    this->port = 9000;
}

Server::Server(int port){
    this->port = port;
}
//inicializa el server comun y corriente
void Server::initializeServer()
{
    descriptorServer = socket(AF_INET, SOCK_STREAM,0);
    serverInfo.sin_family=AF_INET;
    serverInfo.sin_addr.s_addr=htonl(INADDR_ANY);
    serverInfo.sin_port=htons(port);
    idBind = bind(descriptorServer,(struct sockaddr *)&serverInfo, sizeof(serverInfo));

    listen(descriptorServer,10);
    if(descriptorServer==-1 && idBind==-1){
        cout<<"Error iniciando el servidor"<<endl;
    }
}
//metodo estatico donde se maneja toda la comunicacion con el cliente
//en este se deben hacer las respuestas para todos los mensajes enviados por
//los diferentes clientes
void * receiveClient(void* infoClient){
    /*struct parameter *pe = (struct parameter *) infoClient;*/
    ClientInfo * client = /*pe->*/(ClientInfo*)infoClient;
//    vector<ClientInfo*> clients = pe->clients;
    char clientMessage[128];
    while(client->getState()){
        int i= recv(client->getClientDescriptor(),(void *)&clientMessage,128,0);
        sleep(1);
        if(i!=0){
            cout <<"El cliente con IP" <<inet_ntoa(client->getClientInfo().sin_addr) <<" envió: \n";
            cout<<clientMessage <<endl;
//aqui se hace una especie de controlador y se va acomportar segun los mensajes que envie el cliente
            if(strcmp(clientMessage,"1")==0){//si es 1 el mensaje que llego es para hacer como si fuera un cliente
//              hay que decidir a quien se le debe enviar la data de las maquinas de almacenamiento
//              para lo cual hay que mandarles un mensaje para que envien sus atributos y ahi decidir a quien se le manda

//                for(int i = 0; i< clients.size(); i++){
//                    send(clients.at(i)->getClientDescriptor(),(void *)"Envien sus Atribs",sizeof("Envien sus Atribs"),0);
//
//                }

                //una vez se haya decidido a quien enviar los datos se envia la señal de que puede enviar la data
                send(client->getClientDescriptor(),(void *)"Envieme la data",sizeof("Envieme la data"),0);
                //se deve recibir el mensaje y hacer lo de enviar el archivo a la maquina que se debe guardar
            }else if(strcmp(clientMessage,"2") == 0){//si lega 2 es para que se agregue a la lista de clientes pero a ese cliente se le seteara el modo para que sea maquina de almacenamiento
                send(client->getClientDescriptor(),(void *)"Maquina de almacenamiento aceptada",sizeof("Maquina de almacenamiento aceptada"),0);
            }else{
                cout<<"soy un  inutil";
            }

        }else{
            cout<<"Se desconectó el cliente con IP:"<<inet_ntoa(client->getClientInfo().sin_addr) << " con error" <<endl;
            client->setState(false);
            close(client->getClientDescriptor());
        }
    }
}

void Server::acceptClients(){
    int clientDescriptor;
    struct parameter *pe;
    cont = 0;
    while(cont<100){
        struct sockaddr_in clientInfo;
        int socketSize = sizeof(struct sockaddr_in);
        cout<<"Aceptando clientes" <<endl;
        clientDescriptor = accept(this->descriptorServer,(struct sockaddr*)&clientInfo,(socklen_t *) &socketSize);
        if(clientDescriptor!=-1){
            cout<< "Cliente conectado"<<endl;
            clientsDescriptors.push_back(new ClientInfo(clientDescriptor,clientInfo));
            pthread_t clientThread;
/*
            pe->clientInfo = clientsDescriptors[cont];
            pe->clients = getClients();
            cout<<clientsDescriptors[0]->getId();*/
            pthread_create(&clientThread, NULL, &receiveClient,(void *)clientsDescriptors[cont]);
            clientsDescriptors[cont]->setId(cont);
            cont++;
        }
    }
}

void Server::closeServer(void)
{
    exit(EXIT_SUCCESS);
}

void* Server::startServer(void* serverInput){
    Server * server = (Server*) serverInput;
    server->acceptClients();
}

void Server::executeServer(){
    pthread_t thread;
    pthread_create(&thread,NULL,&startServer,(void*)this);
}

void Server::setDescriptorServer(int descriptorServer)
{
    this->descriptorServer = descriptorServer;
}

void Server::setIdBind(int idBind)
{
    this->idBind = idBind;
}

void Server::setPort(int port)
{
    this->port = port;
}

void Server::setServerInfo(struct sockaddr_in serverInfo)
{
    this->serverInfo = serverInfo;
}

int Server::getDescriptorServer()
{
    return this->descriptorServer;
}

int Server::getIdBind()
{
    return this->idBind;
}

int Server::getPort()
{
    return this->port;
}

struct sockaddr_in Server::getServerInfo(){
    return this->serverInfo;
}

vector<ClientInfo *> Server::getClients(){
    return this->clientsDescriptors;
}

