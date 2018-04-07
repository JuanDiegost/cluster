/* 
 * File:   main.cpp
 * Author: JuanDiegost
 *
 * Created on 6 de abril de 2018, 09:51 AM
 */

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main() {
    int client, server;
    int bufsize = 1024;
    int portNum = 1500;
    bool isExit = false;
    char* buffer = new char[bufsize];

    struct sockaddr_in direc;
    socklen_t tamano;
    pid_t pid;

    if ((client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\nSocket server has been created..." << endl;

    direc.sin_family = AF_INET;
    direc.sin_addr.s_addr = htons(INADDR_ANY);
    direc.sin_port = htons(portNum);

    if ((bind(client, (struct sockaddr*) &direc, sizeof (direc))) < 0) {
        cout << "\nError binding connection..." << endl;
        return -1;
    }

    tamano = sizeof (direc);
    cout << "Looking for clients..." << endl;
    listen(client, 1);

    while ((server = accept(client, (struct sockaddr *) &direc, &tamano)) > 0) {
        /*Create the thread and pass the socket descriptor*/

        //Threads
        /*if (pthread_create(new_thread, &thread_attributes, &handle_tcp_connection, (void *) accepted) != 0) {
            perror("create thread");
            exit(EXIT_FAILURE);
        }*/

        strcpy(buffer, "Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "Connected with the client, you are good to go..." << endl;
        cout << "Enter # to end the connection\n" << endl;
        cout << "Enter # to end the connection\n" << endl;

        cout << "Client: ";
        do {
            recv(server, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');

            cout << "Client: ";
            do {
                recv(server, buffer, bufsize, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);

        cout << "\n=> Connection terminated... " << inet_ntoa(direc.sin_addr);
        close(server);
        cout << "\nGoodbye..." << endl;
        isExit = false;
    }

    close(client);
    return 0;
}