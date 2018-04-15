#include <iostream>
#include "Server.h"

using namespace std;

int main(){
    int port = 5000;
    cout<<"Server"<<endl;
//    cout<<"Puerto: "<<endl;
//    cin>>port;
//    fflush(stdin);

    Server server;
    server.setPort(port);
    server.initializeServer();
    server.executeServer();

    while(1);//para que se quede ejecutando el servidor

    return 0;
}
