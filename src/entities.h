#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <ctime>

struct Block {
    Block(int i, std::string h, std::string p)
        : index(i), hash(h), prevHash(p) {}
    int index;
    std::string hash;
    std::string prevHash;
    int amount; 
    time_t timestamp;
    int nonce;
    int difficulty;
};

struct User {
    User(std::string priv, std::string pub, std::string a, double b)
        : privateKey(priv), publicKey(pub), address(a), balance(b) {}
    std::string privateKey;
    std::string publicKey;
    std::string address;
    double balance;
};

#endif