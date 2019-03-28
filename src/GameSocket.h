#ifndef GAME_GAMESOCKET_H
#define GAME_GAMESOCKET_H

#include <iostream>
#include <string.h>

using std::string;

class GameSocket {
private:
    static int counter;
    static bool server;
    static int server_sd;
    static int sd;
public:
    explicit GameSocket(int port);
    explicit GameSocket(char host_name[], int port);
    GameSocket(const GameSocket& to_copy) = delete;
    GameSocket& operator=(const GameSocket&) = delete;
    ~GameSocket();

    string Read();

    void Write(string& send_message);
};


#endif //GAME_GAMESOCKET_H
