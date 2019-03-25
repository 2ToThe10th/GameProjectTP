#ifndef GAME_GAMESOCKET_H
#define GAME_GAMESOCKET_H

#include <iostream>
#include <string.h>

using std::string;

class GameSocket {
private:
    static int counter;
    static GameSocket* exist_object;
    static bool server;
    static int server_sd;
    static int sd;
public:
    GameSocket(int port = 55000);
    GameSocket(char host_name[], int port = 55000);
    GameSocket(const GameSocket& to_copy) = delete;
    GameSocket& operator=(const GameSocket&) = delete;
    ~GameSocket();

    string Read();

    void Write(string& send_message);
};


#endif //GAME_GAMESOCKET_H
