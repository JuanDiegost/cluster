/* 
 * File:   main.cpp
 * Author: JuanDiegost
 *
 * Created on 6 de abril de 2018, 09:47 AM
 */

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
    char a;
    int client;
    int portNum = 1500;
    int bufsize = 1024;
    char* buffer = new char[bufsize];
    bool isExit = false;
    char* ip = "127.0.0.1";

    struct sockaddr_in direc;

    if ((client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "\nError creating socket..." << endl;
        exit(0);
    }

    cout << "\nSocket created successfully..." << endl;
    direc.sin_family = AF_INET;
    direc.sin_port = htons(portNum);
    inet_pton(AF_INET, ip, &direc.sin_addr);

    if (connect(client,(struct sockaddr *)&direc, sizeof(direc)) == 0)
        cout << "Connection to the server " << inet_ntoa(direc.sin_addr) << endl;

    cout << "Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);

    cout << "\n=> Enter # to terminate the connection\n" << endl;

    do {
        cout << "Client: ";
        do {
            cin >> buffer;
            send(client, buffer, bufsize, 0);
            if (*buffer == '#') {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);

        cout << "Server: ";
        do {
            recv(client, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }

        } while (*buffer != 42);
        cout << endl;

    } while (!isExit);
    cout << "=> Connection terminated.\nGoodbye";

    close(client);
    return 0;
}