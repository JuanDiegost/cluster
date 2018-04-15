#include "Client.h"

Client::Client(){
    this->serverPort=9000;
    this->serverIP = "192.168.0.16";
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
    int i;
    char serversMessage[60];
    while(1){
        recv(client->getDescriptor(), (void *) &serversMessage, 60, 0);
        cout<<serversMessage<<endl;
        fflush(stdin);
        switch (atoi( serversMessage)){
    case 1:
        string file;
        cout<<"por favor indica el nombre del archivo: ";
        cin>>file;
        i = send(client->getDescriptor(), (void *) file.c_str(), sizeof(file), 0);
        sleep(1);
        string path="/root/Escritorio/Cluster/Client/";
        path+=file;
        Client::sendFile((void *)client,path.c_str());
    break;
        }
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

inline void *Client::sendFile(void * clientInput, const char *path) {
	#define BLOCK 1024
    Client* client = (Client *) clientInput;
	int size, ofs, nbytes = 0, block = BLOCK;
	char pbuf[BLOCK];
	std::ifstream is;
	is.open (path, std::ios::binary);
	is.seekg (0, std::ios::end);
	size = is.tellg();
	is.seekg (0, std::ios::beg);
	send(client->getDescriptor(), reinterpret_cast<char*>(&size), sizeof size , 0);
	for(ofs = 0; block == BLOCK; ofs += BLOCK) {
		if(size - ofs < BLOCK) block = size - ofs;
		is.read(pbuf, block);
		nbytes += send(client->getDescriptor(), pbuf, block, 0);
		cout<<"block: "<<block<<endl;
	}
	cout<<"archivo enviado"<<endl;
	is.close();
	cout<<"archivo cerrado"<<endl;
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

