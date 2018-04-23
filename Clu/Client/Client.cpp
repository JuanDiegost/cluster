#include "Client.h"

vector<Media *>Client::listMedia;

Client::Client()
{
    this->serverPort=9000;
    this->serverIP = "localhost";
    this->state = true;
    this->isStorageMachine = false;
}

Client::Client(int port, string ip)
{
    this->serverPort = port;
    this->serverIP = ip;
    this->state = true;
    this->isStorageMachine = false;
}

void Client::connectServer()
{
    clientDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    serverInfo.sin_family = AF_INET;
    cout<<"esto lo imprimo yo: "<<serverIP.c_str()<<endl;
    inet_pton(AF_INET, serverIP.c_str(), &serverInfo.sin_addr);
    serverInfo.sin_port = htons(serverPort);
    int conn = connect(clientDescriptor, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
    if(conn!=-1)
    {
        pthread_t writeThread;
        pthread_create(&writeThread, NULL, writeServer, (void *) this);
        while(1);
    }
    else
    {
        cout<<"No se pudo conectar con el servidor"<<endl;
    }
}

//en este menu es donde se hace el menu de acciones y se envian los mensajes con lo que sea que el cliente haya escrito
void * Client::writeServer(void* clientInput)
{
    int name=123;
    Client* client = (Client *) clientInput;
    int conected = 1;
    cout<<"Seleccione una opcion:"<<endl;
    cout<<"1. Acceder como cliente"<<endl;
    cout<<"2. Acceder como maquina de almacenamiento"<<endl;
    char msg[128];
    char serversMessage[128];
    int option;
    string tam;
    bool red=false;
    int option2;
    int i;
    cin>>option;
    fflush(stdin);
    switch (option)
    {
    case 1:
        i = send(client->getDescriptor(), (void *) "4", sizeof("4"), 0);//envia 4 para identificarse como cliente
        while(conected)
        {
            cout<<"Seleccione una opcion:"<<endl;
            cout<<"1. Enviar recurso"<<endl;
            cout<<"2. Obtener lista de almacenamiento"<<endl;
            fflush(stdin);
            cin>>option2;
            switch(option2)
            {
            case 1:
                i = send(client->getDescriptor(), (void *) "1", sizeof(msg), 0);
                recv(client->getDescriptor(), (void *) &serversMessage, sizeof(msg), 0);
                if(strcmp(serversMessage, "2")== 0) //si el server envia un 2 es porque si puede enviar la data
                {
                    i = send(client->getDescriptor(), (void *) "3", sizeof("3"), 0); //se envia 3 para indicar que va a empezar a enviar el archivo
                    string file;
                    cout<<"por favor indica el nombre del archivo: ";
                    cin>>file;
                    sleep(1);
                    i = send(client->getDescriptor(), (void *) file.c_str(), sizeof(file), 0);
                    string path="/root/Escritorio/Client/resources/";
                    path+=file;
                    sleep(5);
                    Client::sendFile((void *)client,path.c_str());
                }
                else
                {
                    cout<<"No se pueden enviar los datos en el momento";
                }
                break;
            case 2:{ //Solicita lista de almacenamiento
                char serversMessage2[10240];
                cout << "solicita lista de almacenamiento";
                send(client->getDescriptor(), (void *) "5", sizeof("5"), 0);
                sleep(1);
                recv(client->getDescriptor(), (void *) &serversMessage2, sizeof(serversMessage2), 0);
                cout << serversMessage2<<endl;
                break;
                }
            }
        }

        break;
      case 2:

        client->setIsStorageMachine(true);
        i = send(client->getDescriptor(), (void *) "2", sizeof("2"), 0);
        recv(client->getDescriptor(), (void *) serversMessage, sizeof(serversMessage), 0);
        system("clear");
        cout<<serversMessage<<endl;
        cout<<"Seleccione un tipo de maquina: "<<endl;
        cout<<"1. Acceder como Imagenes"<<endl;
        cout<<"2. Acceder como Video"<<endl;
        cin>>option2;
        if(option2==1){
            i = send(client->getDescriptor(), (void *) "1", sizeof("1"), 0);
        }else{
            i = send(client->getDescriptor(), (void *) "2", sizeof("2"), 0);
        }

        while(conected)
        {
            while(red){
                sleep(1);
            }
            cout<<"Esperando mensaje del servidor"<<endl;
            recv(client->getDescriptor(), (void *) serversMessage, sizeof(serversMessage), 0);
            red=true;
            cout<<"envio el server: " << serversMessage;
            switch(atoi(serversMessage))
            {
            case 3:  //El servidor solicita el tamaño de añmacenamiento
            {
                cout<<"Solicita tamaño de almacenamiento.."<<endl;

                //tam=std::to_string(Client::getCurrentMachineStorage());
                tam=std::to_string(Client::getSizeStorage());
                cout<<"El tamaño es de: "<<tam<<endl;

                send(client->getDescriptor(), (void *) tam.c_str(), sizeof(tam), 0);
                send(client->getDescriptor(), (void *) tam.c_str(), sizeof(tam), 0);

                //send(client->getDescriptor(), (void *) "0", sizeof("0"), 0);
                //send(client->getDescriptor(), (void *) "0", sizeof("0"), 0);
                red=false;
                break;
            }
            case 4:{//El servidor solicita lista de archivos

                string listN=Client::getListMedia();
                cout << "Tamano: "<<listN.size()<<endl;
                cout << "Lista: "<<listN.c_str();
                send(client->getDescriptor(), (void *) listN.c_str(), listN.size(), 0);
                red=false;
                break;
                }
            case 5:
                cout<<"sending... ";
                    //recv(client->getDescriptor(), (void *) serversMessage, sizeof(serversMessage), 0);


                    string file ="/root/Escritorio/Client/Data/";
                    recv(client->getDescriptor(), (void *) serversMessage, sizeof(serversMessage), 0);
                    cout<<"Name: "<<serversMessage<<endl;
                    file+=serversMessage;
                    cout<<file<<endl;
                    Client::receiveFile((void *)client,file.c_str());
                    cout<<"YA"<<endl;
                    sleep(5);
                    red=false;
                break;
            }
        }
        break;
    default:
        red=false;
        break;
        sleep(1);
        if(i==-1)
        {
            conected = 0;
            cout<<"se desconecto del servidor"<<endl;
            close(client->getDescriptor());
            exit(EXIT_SUCCESS);
        }
    }
}

string Client::getListMedia(){
    string result="";
    for(int i=0;i<Client::listMedia.size();i++){
        result += Client::listMedia.at(i)->toString()+ "\n" ;
    }
    return result;
}

int Client::getSizeStorage(){
    int sizeM=0;
    for(int i=0;i<listMedia.size();i++){
        sizeM+=listMedia.at(i)->getSizeFile();
    }
    return sizeM;
}

inline void  *Client::receiveFile(void* infoClient, const char *path) {
	#define BLOCKC 1024
    Client * client =(Client*)infoClient;
	int size, ofs, nbytes = 0, block = BLOCKC;
	char pbuf[BLOCKC];
	std::ofstream os(path, std::ios::binary);
	recv(client->getDescriptor(), reinterpret_cast<char*>(&size), sizeof size, 0);
	cout<<"recibiendo"<<endl;
	for(ofs = 0; block == BLOCKC; ofs += BLOCKC) {
        //sleep(1);
		if(size - ofs < BLOCKC) block = size - ofs;
		nbytes += recv(client->getDescriptor(), pbuf, block, 0);
		os.write(pbuf, block);
		cout<<"block= "<<block<<endl;
	}
	os.close();

	Media * media=new Media(path,nbytes);
	listMedia.push_back(media);
    cout<<"completado"<<endl;

}


inline void *Client::sendFile(void * clientInput, const char *path)
{
#define BLOCK 10240
    Client* client = (Client *) clientInput;
    int size, ofs, nbytes = 0, block = BLOCK;
    char pbuf[BLOCK];
    std::ifstream is;
    is.open (path, std::ios::binary);
    is.seekg (0, std::ios::end);
    size = is.tellg();
    is.seekg (0, std::ios::beg);
    send(client->getDescriptor(), reinterpret_cast<char*>(&size), sizeof size, 0);
    for(ofs = 0; block == BLOCK; ofs += BLOCK)
    {
        //sleep(1);//Parece que el server no puede ller muy rapido la info q le llega y las partes que le faltan le llegan por soy un inutil... la linea del else con esto se soluciona pero es muy lento para enviar
        if(size - ofs < BLOCK) block = size - ofs;
        is.read(pbuf, block);
        nbytes += send(client->getDescriptor(), pbuf, block, 0);
        cout<<"block: "<<block<<endl;
    }
    cout<<"archivo enviado"<<endl;
    is.close();
    cout<<"archivo cerrado"<<endl;
}



int Client::getDescriptor(void)
{
    return this->clientDescriptor;
}

int Client::getFileSize(std::string fileName)
{
    FILE *p_file = NULL;
    p_file = fopen(fileName.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int mySize = ftell(p_file);
    fclose(p_file);
    return mySize;
}

int Client::getCurrentMachineStorage()
{
    int totalSizeInBytes = 0;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir("/root/Escritorio/Client/Data/"))!= NULL)
    {
        while((ent=readdir(dir))!=NULL)
        {
            int fileSize =0;
            if((strcmp(ent->d_name,".") != 0) && (strcmp(ent->d_name,"..") !=0 ))
            {
                fileSize =  Client::getFileSize(ent->d_name);
                printf("%s -> %d\n", ent->d_name, fileSize);
                totalSizeInBytes += fileSize;
            }
        }
        closedir(dir);
    }
    else
    {
        perror("No ha sido posible leer el directorio de almacenamiento");
    }
    cout<<"size totl: "<<totalSizeInBytes;
    return totalSizeInBytes;
}

void Client::setDescriptor(int descriptor)
{
    this->clientDescriptor = descriptor;
}

bool Client::getState(void)
{
    return this->state;
}

void Client::setState(bool state)
{
    this->state = state;
}

bool Client::getIsStorageMachine(void)
{
    return this->isStorageMachine;
}

void Client::setIsStorageMachine(bool mode)
{
    this->isStorageMachine = mode;
}
