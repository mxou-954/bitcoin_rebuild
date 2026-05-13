#include <fstream>
#include <string>
#include <iostream>
#include "write_and_read.h"

// On exporte dans les registres les users, les utxos, les blocks

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

void export_user_in_registre(std::string path, std::vector<User>& users){
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

std::vector<User> read_users(std::string path){
    std::vector<User> users;
    
    // 1. Ouvrir le fichier
    std::ifstream file(path);
    if(!file.is_open()) return users;
    
    // 2. Parser le texte en JSON
    json j;
    file >> j;
    file.close();
    
    // 3. Maintenant tu peux itérer
    for(auto& item : j){
        std::vector<UTXO> utxos;
        for(auto& u : item["utxos"]){
            UTXO utxo(
                u["amount"],
                u["public_key"],
                u["txid_transaction"],
                u["vout"]
            );
            utxos.push_back(utxo);
        }
        User user(
            item["privateKey"],
            item["publicKey"],
            item["address"],
            item["balance"],
            utxos
        );
        users.push_back(user);
    }
    
    return users;
}

void export_blockchain_in_registre(std::string path, std::vector<Block>& blocks) {
    json arr = json::array();
    
    for(Block block : blocks) {
        json j;
        j["index"]      = block.index;
        j["hash"]       = block.hash;
        j["prevHash"]   = block.prevHash;
        j["timestamp"]  = block.timestamp;
        j["nonce"]      = block.nonce;
        j["difficulty"] = block.difficulty;
        j["resolved"]   = block.resolved;
    
        json transactions = json::array();
        for(Transaction transaction : block.transactions){
            json t;
            t["txid"]            = transaction.txid;
            t["timestamp"]       = transaction.timestamp;
            t["value_btc"]       = transaction.value_btc;
            t["fee"]             = transaction.fee;
            t["vsize"]           = transaction.vsize;
            t["fee_per_vbyte"]   = transaction.fee_per_vbyte;
            t["nb_input"]        = transaction.nb_input;
            t["nb_output"]       = transaction.nb_output;
            t["nb_adresses_src"] = transaction.nb_adresses_src;
        
            // Adresses sources
            json adresses_sources = json::array();
            for(std::string a : transaction.adresses_sources){
                adresses_sources.push_back(a);
            }
            t["adresses_sources"] = adresses_sources;
        
            // Adresses dest
            json adresses_dest = json::array();
            for(std::string a : transaction.adresses_dest){
                adresses_dest.push_back(a);
            }
            t["adresses_dest"] = adresses_dest;
        
            // Input UTXOs
            json input_utxos = json::array();
            for(UTXO u : transaction.input_utxos){
                json utxo;
                utxo["amount"]           = u.amount;
                utxo["public_key"]       = u.public_key;
                utxo["txid_transaction"] = u.txid_transaction;
                utxo["vout"]             = u.vout;
                input_utxos.push_back(utxo);
            }
            t["input_utxos"] = input_utxos;
        
            // Output UTXOs
            json output_utxos = json::array();
            for(UTXO u : transaction.output_utxos){
                json utxo;
                utxo["amount"]           = u.amount;
                utxo["public_key"]       = u.public_key;
                utxo["txid_transaction"] = u.txid_transaction;
                utxo["vout"]             = u.vout;
                output_utxos.push_back(utxo);
            }
            t["output_utxos"] = output_utxos;
        
            transactions.push_back(t);
        }
        j["transactions"] = transactions;
        arr.push_back(j);
    }

    std::ofstream file(path);
    file << arr.dump(4);
    file.close();
}

// ============ BLOCKCHAIN ============

std::vector<Block> read_blockchain(std::string path) {
    std::vector<Block> blocks;
    std::ifstream file(path);
    if(!file.is_open()) return blocks;
    
    json arr;
    file >> arr;
    file.close();
    
    for(auto& b : arr){
        Block block;
        block.index      = b["index"];
        block.hash       = b["hash"];
        block.prevHash   = b["prevHash"];
        block.timestamp  = b["timestamp"];
        block.nonce      = b["nonce"];
        block.difficulty = b["difficulty"];
        block.resolved   = b["resolved"];
        
        for(auto& t : b["transactions"]){
            Transaction transaction;
            transaction.txid            = t["txid"];
            transaction.timestamp       = t["timestamp"];
            transaction.value_btc       = t["value_btc"];
            transaction.fee             = t["fee"];
            transaction.vsize           = t["vsize"];
            transaction.fee_per_vbyte   = t["fee_per_vbyte"];
            transaction.nb_input        = t["nb_input"];
            transaction.nb_output       = t["nb_output"];
            transaction.nb_adresses_src = t["nb_adresses_src"];
            
            for(auto& a : t["adresses_sources"])
                transaction.adresses_sources.push_back(a);
            
            for(auto& a : t["adresses_dest"])
                transaction.adresses_dest.push_back(a);
            
            for(auto& u : t["input_utxos"])
                transaction.input_utxos.push_back(UTXO(u["amount"], u["public_key"], u["txid_transaction"], u["vout"]));
            
            for(auto& u : t["output_utxos"])
                transaction.output_utxos.push_back(UTXO(u["amount"], u["public_key"], u["txid_transaction"], u["vout"]));
            
            block.transactions.push_back(transaction);
        }
        blocks.push_back(block);
    }
    return blocks;
}

// ============ MEMPOOL ============

void export_mempool_in_registre(std::string path, Mempool& mempool) {
    json arr = json::array();
    
    for(Transaction t : mempool.transactions){
        json transaction;
        transaction["txid"]            = t.txid;
        transaction["timestamp"]       = t.timestamp;
        transaction["value_btc"]       = t.value_btc;
        transaction["fee"]             = t.fee;
        transaction["vsize"]           = t.vsize;
        transaction["fee_per_vbyte"]   = t.fee_per_vbyte;
        transaction["nb_input"]        = t.nb_input;
        transaction["nb_output"]       = t.nb_output;
        transaction["nb_adresses_src"] = t.nb_adresses_src;
        
        json adresses_sources = json::array();
        for(std::string a : t.adresses_sources)
            adresses_sources.push_back(a);
        transaction["adresses_sources"] = adresses_sources;
        
        json adresses_dest = json::array();
        for(std::string a : t.adresses_dest)
            adresses_dest.push_back(a);
        transaction["adresses_dest"] = adresses_dest;
        
        json input_utxos = json::array();
        for(UTXO u : t.input_utxos){
            json utxo;
            utxo["amount"]           = u.amount;
            utxo["public_key"]       = u.public_key;
            utxo["txid_transaction"] = u.txid_transaction;
            utxo["vout"]             = u.vout;
            input_utxos.push_back(utxo);
        }
        transaction["input_utxos"] = input_utxos;
        
        json output_utxos = json::array();
        for(UTXO u : t.output_utxos){
            json utxo;
            utxo["amount"]           = u.amount;
            utxo["public_key"]       = u.public_key;
            utxo["txid_transaction"] = u.txid_transaction;
            utxo["vout"]             = u.vout;
            output_utxos.push_back(utxo);
        }
        transaction["output_utxos"] = output_utxos;
        
        arr.push_back(transaction);
    }
    
    std::ofstream file(path);
    file << arr.dump(4);
    file.close();
}

Mempool read_mempool(std::string path) {
    Mempool mempool;
    std::ifstream file(path);
    if(!file.is_open()) return mempool;
    
    json arr;
    file >> arr;
    file.close();
    
    for(auto& t : arr){
        Transaction transaction;
        transaction.txid            = t["txid"];
        transaction.timestamp       = t["timestamp"];
        transaction.value_btc       = t["value_btc"];
        transaction.fee             = t["fee"];
        transaction.vsize           = t["vsize"];
        transaction.fee_per_vbyte   = t["fee_per_vbyte"];
        transaction.nb_input        = t["nb_input"];
        transaction.nb_output       = t["nb_output"];
        transaction.nb_adresses_src = t["nb_adresses_src"];
        
        for(auto& a : t["adresses_sources"])
            transaction.adresses_sources.push_back(a);
        
        for(auto& a : t["adresses_dest"])
            transaction.adresses_dest.push_back(a);
        
        for(auto& u : t["input_utxos"])
            transaction.input_utxos.push_back(UTXO(u["amount"], u["public_key"], u["txid_transaction"], u["vout"]));
        
        for(auto& u : t["output_utxos"])
            transaction.output_utxos.push_back(UTXO(u["amount"], u["public_key"], u["txid_transaction"], u["vout"]));
        
        mempool.transactions.push_back(transaction);
    }
    return mempool;
}