#include "CryptoSocket.h"

CryptoSocket::CryptoSocket(int port) : GameSocket(port) {}

CryptoSocket::CryptoSocket(char *host_name, int port) : GameSocket(host_name, port) {}

string CryptoSocket::encode(string str) {
    string ret;
    for(auto i: str) {
        if(i >= 'a' && i <= 'z') {
            int id = i - 'a';
            id = (id + key) % 26;
            ret.push_back(char(id +'a'));
        }
        else {
            ret.push_back(i);
        }
    }

    return ret;
}
string CryptoSocket::decode(string str) {
    string ret;
    for(auto i: str) {
        if(i >= 'a' && i <= 'z') {
            int id = i - 'a';
            id = (26 + id - key) % 26;
            ret.push_back(char(id +'a'));
        }
        else {
            ret.push_back(i);
        }
    }

    return ret;
}

string CryptoSocket::Read() {
    return decode(GameSocket::Read());
}

void CryptoSocket::Write(string &send_message) {
    string crypto_send_message = encode(send_message);
    GameSocket::Write(crypto_send_message);
}
