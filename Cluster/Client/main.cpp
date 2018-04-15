#include <iostream>
#include "Client.h"
using namespace std;

int main()
{
    int port = 5000;
    cout<<"Cliente"<<endl;
//    cout<<"Puerto: "<<endl;
//    cin>>port;

    cout<<"Nombre del host o ip: ";
    string host;
    cin>>host;
    fflush(stdin);

    Client *client = new Client(port, host);
    client->connectServer();
    return 0;
}
