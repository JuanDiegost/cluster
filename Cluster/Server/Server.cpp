#include "Server.h"

struct parameter {
    ClientInfo * clientInfo;
    vector<ClientInfo*> clients;
};

vector<ClientInfo *>Server::clientsDescriptors;

Server::Server(){
    this->port = 9000;
    this->cont = 0;
    }

Server::Server(int port){
    this->port = port;
    this->cont = 0;
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
    ClientInfo * client = (ClientInfo*)infoClient;
    int option=0;
    char clientMessage[128];
    while(client->getState()){
        int i= recv(client->getClientDescriptor(),(void *)&clientMessage,128,0);
        sleep(1);
        if(i!=0){
            cout <<"El cliente con IP" <<inet_ntoa(client->getClientInfo().sin_addr) <<" envi�:" << clientMessage<<endl;

            option=atoi(clientMessage);
            switch (option){
            case 1:
                send(client->getClientDescriptor(),(void *)"2",sizeof("2"),0);//envia aprovacion para enviar la data
                cout<<"Aprobacion aceptada"<<endl;
            break;
            case 2://si lega 2 es para que se agregue a la lista de clientes pero a ese cliente se le seteara el modo para que sea maquina de almacenamiento
                send(client->getClientDescriptor(),(void *)"Maquina de almacenamiento aceptada",sizeof("Maquina de almacenamiento aceptada"),0);
                client->setType("Almacenamiento");
                Server::clientsDescriptors.push_back(client);
            break;
            case 3:{
                    cout<<"sending... ";
                    i= recv(client->getClientDescriptor(),(void *)&clientMessage,128,0);
                    string file ="/root/Escritorio/Cluster/Server/data/";
                    file+=clientMessage;

                    Server::receiveFile((void *)infoClient,file.c_str());

                    cout<<"Maquina count"<<Server::clientsDescriptors.size()<<endl;

                    for(int j = 0; j < Server::clientsDescriptors.size(); j++){
                        cout<<"enviado tam "<<j<<endl;
                        if(Server::clientsDescriptors.at(j)->getType()=="Almacenamiento"){
                        send(Server::clientsDescriptors.at(j)->getClientDescriptor(),(void *)"3",sizeof("3"),0);
                        cout << "envie a"<<j<<endl;
                        }
                       //Cuando hay varai maquinas de almacenamiento el servidor no sigue con el for hata que
                       //se desconecte la que envio por eso hay qu primero enviar a todas y luegorecibr a todas
                    }
                    cout<<"sali for"<<endl;
                        for(int j = 0; j < Server::clientsDescriptors.size(); j++){
                        cout<<"reci :"<<j<<endl;
                        if(Server::clientsDescriptors.at(j)->getType()=="Almacenamiento"){
                        i= recv(Server::clientsDescriptors.at(j)->getClientDescriptor(),(void *)&clientMessage,128,0);
                        cout <<"El cliente con " << j <<" tiene :" << clientMessage<<endl;
                        }

                    }
                    cout<<"sali for"<<endl;

                break;
            }
            case 4://conectar como cliente
                client->setType("Cliente");
                Server::clientsDescriptors.push_back(client);
                break;
            }

        }else{
            cout<<"Se desconect� el cliente con IP:"<<inet_ntoa(client->getClientInfo().sin_addr) << " con error" <<endl;
            client->setState(false);
            close(client->getClientDescriptor());
        }
    }
}

inline void  *Server::receiveFile(void* infoClient, const char *path) {
	#define BLOCK 10240
    ClientInfo * client = /*pe->*/(ClientInfo*)infoClient;
	int size, ofs, nbytes = 0, block = BLOCK;
	char pbuf[BLOCK];
	std::ofstream os(path, std::ios::binary);
	recv(client->getClientDescriptor(), reinterpret_cast<char*>(&size), sizeof size, 0);
	cout<<"recibiendo"<<endl;
	for(ofs = 0; block == BLOCK; ofs += BLOCK) {
		if(size - ofs < BLOCK) block = size - ofs;
		nbytes += recv(client->getClientDescriptor(), pbuf, block, 0);
		os.write(pbuf, block);
		cout<<"block= "<<block<<endl;
	}
    cout<<"completado"<<endl;
	os.close();
    cout<<"cerrado"<<endl;

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
            ClientInfo *ci = new ClientInfo(clientDescriptor,clientInfo);
//            cout<<"este es el id"<<ci->getId();

              pthread_t clientThread;
//            pe->clientInfo = clientsDescriptors[cont];
//            pe->clients = getClients();

            pthread_create(&clientThread, NULL, &receiveClient,(void *)ci);
//            pthread_create(&clientThread, NULL, &receiveClient,(void *)&pe);
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

