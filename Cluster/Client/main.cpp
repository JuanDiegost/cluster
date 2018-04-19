#include <iostream>
#include <string>
#include <dirent.h>
#include "Client.h"
//#include <filesystem>
//namespace fs = std::filesystem;
using namespace std;



int main()
{
    int port = 7000;
    cout<<"Cliente"<<endl;
    //cout<<"Puerto: "<<endl;
    //cin>>port;

    cout<<"Nombre del host o ip: ";
    string host;
    cin>>host;
    fflush(stdin);

    Client *client = new Client(port, host);
    client->connectServer();



}
