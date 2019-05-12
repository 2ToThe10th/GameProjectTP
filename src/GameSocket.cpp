#include "GameSocket.h"
#include <sys/socket.h>
#include <resolv.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "errno.h"

using std::string;
using std::cout;
using std::endl;
using std::perror;

int GameSocket::counter = 0;
bool GameSocket::server = false;
int GameSocket::server_sd = 0;
int GameSocket::sd = 0;

GameSocket::GameSocket(int port) {

    if(counter) {
        ++counter;
        return;
    }

    ++counter;
    server = true;

    if(port <= 0 || port >= 65536) {
        cout << "Port might be between 1 and 65535" << endl;
        counter = 0;
        throw -9;
    }

    sockaddr_in addr;
    bzero((void*)(&addr), sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    char host_name[1000];
    gethostname(host_name, 999);
    inet_aton(host_name, &addr.sin_addr);

    server_sd = socket(PF_INET, SOCK_STREAM, 0);

    if(bind(server_sd, (sockaddr*)&addr, sizeof(addr)) != 0) {
        perror("error bind");
        cout << "This port is not free. Try to choose another port or wait while this port will became free." << endl;
        counter = 0;
        throw -1;
    }

    if(listen(server_sd, 5) != 0) {
        perror("error listen" );
        counter = 0;
        throw -2;
    }

    cout << "Waiting connection on hostname: " << host_name << " port: " << port << endl;

    unsigned int size_of_addr = sizeof(addr);
    sd = accept(server_sd, (sockaddr*)&addr, &size_of_addr);
}

GameSocket::GameSocket(char host_name[], int port) {
    if(counter) {
        ++counter;
        return;
    }

    ++counter;
    server = false;

    if(port <= 0 || port >= 65536) {
        cout << "Port might be between 1 and 65535" << endl;
        counter = 0;
        throw -9;
    }

    sockaddr_in dest;
    bzero((void*)(&dest), sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_aton(host_name, &dest.sin_addr);

    sd = socket(PF_INET, SOCK_STREAM, 0);

    if(connect(sd, (sockaddr*)&dest, sizeof(dest)) != 0) {
        perror("error connect" );
        counter = 0;
        throw -3;
    }
}

GameSocket::~GameSocket() {
    --counter;

    if(!counter) {

        if(close(sd) != 0) {
            perror("socket is not closed");
        }

        if(server) {
            if( close(server_sd) != 0) {
                perror("server socket is not closed");
            }
        }
    }
}

string GameSocket::Read() {
    const int MAX_MESSAGE_SIZE = 10000;
    char char_message[MAX_MESSAGE_SIZE + 10];

    if(read(sd, char_message, MAX_MESSAGE_SIZE) == -1) {
        perror("error while reading socket");
    }

    string message;

    for(int i = 0; i < MAX_MESSAGE_SIZE; ++i) {
        if(char_message[i] == '\0') {
            break;
        } else {
            message += char_message[i];
        }
    }

    return message;
}

void GameSocket::Write(string &send_message) {
    const unsigned long MAX_MESSAGE_SIZE = send_message.size();
    char char_message[MAX_MESSAGE_SIZE + 10];
    for(unsigned long i = 0; i < MAX_MESSAGE_SIZE; ++i) {
        char_message[i] = send_message[i];
    }
    char_message[MAX_MESSAGE_SIZE] = '\0';

    if(write(sd, char_message, MAX_MESSAGE_SIZE + 1) == -1) {
        perror("error while writing to socket");
    }
}


