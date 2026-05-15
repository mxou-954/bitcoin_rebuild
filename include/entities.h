#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <ctime>
#include <vector>

struct UTXO {
    UTXO(double amount, std::string public_key, std::string txid_transaction, int vout)
        : amount(amount), public_key(public_key), txid_transaction(txid_transaction), vout(vout) {}
    double amount; 
    std::string public_key;
    std::string txid_transaction;
    int vout;
};

struct Transaction {
    Transaction() {}

    Transaction(std::string txid, time_t timestamp, double value_btc, 
        double fee, double vsize, double fee_per_vbyte, int nb_input, int nb_output,
        int nb_adresses_src, std::vector<std::string> adresses_sources, 
        std::vector<std::string> adresses_dest, std::vector<UTXO> input_utxos, std::vector<UTXO> output_utxos)
        : txid(txid), timestamp(timestamp), value_btc(value_btc), fee(fee), vsize(vsize),
        fee_per_vbyte(fee_per_vbyte), nb_input(nb_input), nb_output(nb_output), 
        nb_adresses_src(nb_adresses_src), adresses_sources(adresses_sources), adresses_dest(adresses_dest),
        input_utxos(input_utxos), output_utxos(output_utxos) {}
    std::string txid; 
    time_t timestamp;
    double value_btc;
    double fee;
    double vsize;
    double fee_per_vbyte;
    int nb_input;
    int nb_output;
    int nb_adresses_src;
    std::vector<std::string> adresses_sources;
    std::vector<std::string> adresses_dest;
    std::vector<UTXO> input_utxos;
    std::vector<UTXO> output_utxos;
};

struct Block {
    Block() {}

    Block(int index, std::string hash, std::string prevHash, time_t timestamp,
    int nonce, int difficulty, std::vector<Transaction> transaction, bool resolved)
        : index(index), hash(hash), prevHash(prevHash), timestamp(timestamp), nonce(nonce),
        difficulty(difficulty), transactions(transactions), resolved(resolved) {}
    int index = 0;
    std::string hash = "";
    std::string prevHash = "";
    time_t timestamp = 0;
    int nonce = 0;
    int difficulty = 0;
    std::vector<Transaction> transactions = {};
    bool resolved = false;
};

struct User {
    User(std::string priv, std::string pub, std::string a, double b, std::vector<UTXO> utxos)
        : privateKey(priv), publicKey(pub), address(a), balance(b), utxos(utxos) {}
    std::string privateKey;
    std::string publicKey;
    std::string address;
    double balance;
    std::vector<UTXO> utxos;
};

struct Mempool {
    Mempool() {}

    Mempool(std::vector<Transaction> transactions, int max_size = 300)
        : transactions(transactions), max_size(max_size) {}
    std::vector<Transaction> transactions;
    int max_size = 300;
};

struct Peer {
    Peer() {}

    Peer(std::string ip, int port, bool connected)
        : ip(ip), port(port), connected(connected) {}
    std::string ip;
    int port;
    bool connected;
};

struct Node {
    Node() {}

    Node(std::string ip, int port, std::vector<Peer>& peers)
        : ip(ip), port(port), peers(peers) {}
    std::string ip;
    int port;
    std::vector<Peer> peers;
};

#endif