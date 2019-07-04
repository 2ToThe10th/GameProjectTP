#ifndef GAME_SRC_CRYPTOSOCKET_H_
#define GAME_SRC_CRYPTOSOCKET_H_

#include "GameSocket.h"

using std::string;

class CryptoSocket: public GameSocket {
private:
    GameSocket* socket;
    const int key = 8;
    string encode(string str);
    string decode(string str);
public:
    explicit CryptoSocket(int port);
    explicit CryptoSocket(char host_name[], int port);

    string Read() override;
    void Write(string& send_message) override;
};

#endif //GAME_SRC_CRYPTOSOCKET_H_
