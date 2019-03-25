#include "GameSocket.h"
#include <sys/socket.h>
#include <resolv.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using std::string;
using std::cout;
using std::endl;

int GameSocket::counter = 0;
GameSocket* GameSocket::exist_object = nullptr;
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

    sockaddr_in addr;
    bzero((void*)(&addr), sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    char host_name[1000];
    gethostname(host_name, 999);
    inet_aton(host_name, &addr.sin_addr);

    server_sd = socket(PF_INET, SOCK_STREAM, 0);

    if(bind(server_sd, (sockaddr*)&addr, sizeof(addr)) != 0) {
        cout << "error bind" << endl;
        throw -1;
    }

    if(listen(server_sd, 5) != 0) {
        cout << "error listen" << endl;
        throw -2;
    }

    cout << "waiting connection on hostname: " << host_name << " port: " << port << endl;

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

    sockaddr_in dest;
    bzero((void*)(&dest), sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_aton(host_name, &dest.sin_addr);

    sd = socket(PF_INET, SOCK_STREAM, 0);

    if(connect(sd, (sockaddr*)&dest, sizeof(dest)) != 0) {
        cout << "error connect" << endl;
        throw -3;
    }

    cout << "CONNECT" << endl;
}

GameSocket::~GameSocket() {
    --counter;

    if(!counter) {

        close(sd);

        if(server) {
            close(server_sd);
        }
    }
}

string GameSocket::Read() {
    const int MAX_MESSAGE_SIZE = 10000;
    char char_message[MAX_MESSAGE_SIZE + 10];

    if(read(sd, char_message, MAX_MESSAGE_SIZE) == -1) {
        cout << "error read" << endl;
        throw -4;
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
    const int MAX_MESSAGE_SIZE = send_message.size();
    char char_message[MAX_MESSAGE_SIZE + 10];
    for(int i = 0; i < MAX_MESSAGE_SIZE; ++i) {
        char_message[i] = send_message[i];
    }
    char_message[MAX_MESSAGE_SIZE] = '\0';

    if(write(sd, char_message, MAX_MESSAGE_SIZE + 1) == -1) {
        cout << "error write" << endl;
        throw -5;
    }

    return;
}

