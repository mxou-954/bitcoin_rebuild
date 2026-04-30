#include <fstream>
#include <string>
#include <iostream>
#include "write_and_read.h"

// ifstream c'est lecture seulement, crée pas si c'est inexistant
// ofstream c'est écriture, crée automatiquement si il n'existe pas et écrase le contenu par défaut

void is_file_exist(std::string path) {
    std::ifstream file(path);
    if(!file.is_open()){
        std::cout << "Fichier introuvable ! Création de ce dernier..." << std::endl;
        std::ofstream new_file(path);
        new_file.close();
    }
    file.close();
}

void export_user_in_registre(std::string path, std::vector<User> users){
    json arr = json::array();
    
    for(User user : users){
        json j;
        j["address"]    = user.address;
        j["balance"]    = user.balance;
        j["privateKey"] = user.privateKey;
        j["publicKey"]  = user.publicKey;
        
        json utxos = json::array();
        for(UTXO utxo : user.utxos){
            json u;
            u["amount"]           = utxo.amount;
            u["public_key"]       = utxo.public_key;
            u["txid_transaction"] = utxo.txid_transaction;
            u["vout"]             = utxo.vout;
            utxos.push_back(u);
        }
        j["utxos"] = utxos;
        arr.push_back(j);
    }
    
    std::ofstream file(path);
    file << arr.dump(4);
    file.close();
}