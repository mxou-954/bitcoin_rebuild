#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <ctime>

struct Block {
    Block(int i, std::string h, std::string p, int amount, time_t timestamp,
    int nonce, int difficulty, std::vector<Transaction> transaction)
        : index(i), hash(h), prevHash(p), amount(amount), timestamp(timestamp), nonce(nonce),
        difficulty(difficulty), transaction(transaction) {}
    int index;
    std::string hash;
    std::string prevHash;
    int amount; 
    time_t timestamp;
    int nonce;
    int difficulty;
    std::vector<Transaction> transaction;
};

struct UTXO {
    UTXO(double amount, std::string public_key, std::string txid_transaction, int vout)
        : amount(amount), public_key(public_key), txid_transaction(txid_transaction), vout(vout) {}
    double amount; 
    std::string public_key;
    std::string txid_transaction;
    int vout;
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

struct Transaction {
    Transaction(std::string txid, time_t timestamp, double value_btc, 
        double fee, double vsize, double fee_per_vbyte, int nb_input, int nb_output,
        int nb_adresses_src, std::vector<std::string> adresses_sources, 
        std::vector<std::string> adresses_dest)
        : txid(txid), timestamp(timestamp), value_btc(value_btc), fee(fee), vsize(vsize),
        fee_per_vbyte(fee_per_vbyte), nb_input(nb_input), nb_output(nb_output), 
        nb_adresses_src(nb_adresses_src), adresses_sources(adresses_sources), adresses_dest(adresses_dest) {}
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
};

#endif