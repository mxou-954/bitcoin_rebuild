#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>

#include "entities.h"
#include "generate_random_number.h"
#include "sha256.h"
#include "new_user.h"
#include "split.h"
#include "btc_to_satochi.h" 
#include "is_currentUser.h"
#include "find_user.h"
#include "utxos.h"
#include "block.h"
#include "sync_user.h"
#include "help.h"
#include "write_and_read.h"
#include "security_check_for_pay.h"

#include <openssl/sha.h>

using namespace std;

int main() {
    srand(time(0));

    std::vector<User> users    = read_users("../registre/users.json");
    std::vector<Block> blocks   = read_blockchain("../registre/blockchain.json");
    Mempool mempool  = read_mempool("../registre/mempool.json");

    User currentUser = User("", "", "", 0.0, {});

    for(int i =0; i<3; i++){
        users.push_back(new_user());
    };

    for(User u : users){
        std::cout << "\n";
        std::cout << "==========================================" << "\n";
        std::cout << "user_address: " << u.address << "\n";
        std::cout << "user_privateKey: " << u.privateKey << "\n";
        std::cout << "user_publicKey: " << u.publicKey << "\n";
        std::cout << "user_balance: " << u.balance << "\n";
        std::cout << "==========================================" << "\n";
    };
    
    while(true) {

        // Controller de sécurité //
        bool is_connected = is_currentUser(currentUser);
        ////////////////////////////

        std::string user_command = "";
        std::string delimiter = " ";

        if (currentUser.address == ""){
            std::cout << "\n" << "guest >> ";
        } else {
            std::cout << "\n" << currentUser.address << " >> ";
        }
        
        std::getline(std::cin, user_command);
        std::vector<std::string> user_command_part = split(user_command, delimiter);


        // NECESSITE D'ETRE CONNECTE 
        if(is_connected){
            if(user_command_part[0] == "/pay") {
                // /pay [publicKey] [amount] [address]
                // prendre amount en BTC

                std::optional<User> match_address = find_user_by_address(user_command_part[3], users);
                std::optional<User> match_public_key = find_user_by_public_key(user_command_part[1], users);

                if(!security_check_for_pay(currentUser, match_address, match_public_key, user_command_part)){
                    continue;
                }

                double addition = 0.0;
                std::vector<UTXO> to_use;
                std::vector<UTXO> currentUser_utxos = currentUser.utxos;
                while(addition < stod(user_command_part[2]) && !currentUser_utxos.empty()){
                    UTXO utxo = currentUser_utxos.back();
                    addition = addition + utxo.amount;
                    to_use.push_back(utxo);
                    currentUser_utxos.pop_back();
                }

                time_t timestamp = time(nullptr);
                currentUser.utxos = currentUser_utxos;
                double amount_output = addition - stod(user_command_part[2]);

                std::stringstream ss;
                ss << timestamp << currentUser.address << user_command_part[3] << user_command_part[2];
                std::string txid = sha256(ss.str());

                // Changer le generate number par le vrai id de la transaction
                UTXO to_sender = create_utxo(currentUser, amount_output, txid, 0); 
                UTXO to_receiver = create_utxo(match_address.value(), stod(user_command_part[2]), txid, 1);

                std::vector<UTXO> to_return;
                to_return.push_back(to_sender);
                to_return.push_back(to_receiver);

                double vsize = 160.0; // vbytes
                double fee_per_vbyte = 10.0; // sat/vbytes
                std::vector<std::string> adresses_sources;
                std::vector<std::string> adresses_dest;

                adresses_sources.push_back(currentUser.address);
                adresses_dest.push_back(user_command_part[2]);
                
                Transaction new_transaction = Transaction(
                    txid, 
                    time(&timestamp),
                    btc_to_satochi(stod(user_command_part[2])),
                    vsize * fee_per_vbyte,
                    vsize, 
                    fee_per_vbyte,
                    to_use.size(), // nb_input
                    0, // nb_output
                    0, // nb_adresses_src
                    adresses_sources,
                    adresses_dest, 
                    to_use, 
                    to_return
                );

                match_address.value().utxos.push_back(to_receiver);
                currentUser.utxos.push_back(to_sender);

                match_address.value().balance = utxos_sum_balance(match_address.value());
                currentUser.balance = utxos_sum_balance(currentUser);

                sync_user(users, currentUser);
                sync_user(users, match_address.value());

                if (mempool.transactions.size() < mempool.max_size){
                    mempool.transactions.push_back(new_transaction);
                }

                export_user_in_registre("../registre/users.json", users);
                export_mempool_in_registre("../registre/mempool.json", mempool);
                
            } else if (user_command_part[0] == "/transfer") {
                // /transfer [euros] [publicKey]
                if(sha256(currentUser.privateKey) != user_command_part[2]){
                    std::cout << "La clé public n'est pas celle lié a votre compte." << std::endl;
                    continue;
                }

                double euros = stod(user_command_part[1]);
                double btc_price = 85000.0; // prix constant pour l'instant
                double montant_btc = euros / btc_price;
                double montant_satoshis = btc_to_satochi(montant_btc);

                UTXO new_utxo = create_utxo(currentUser, montant_satoshis, generate_random_number(50), 0);

                currentUser.utxos.push_back(new_utxo);
                currentUser.balance += montant_btc;
                sync_user(users, currentUser);
                export_user_in_registre("../registre/users.json", users);
                
            } else if (user_command_part[0] == "/mine") {
                // /mine [nonce]
                bool result = mine(blocks, stoi(user_command_part[1]), mempool);
                
            } else if (user_command_part[0] == "/blockchain") {
                // /blockchain
                for(const Block& block : blocks){
                    view_block(block);
                }
                
            } else if (user_command_part[0] == "/find_block_by_index") {
                // /find_block_by_index [index]
                std::optional<Block> result = find_block_by_index(blocks, stoi(user_command_part[1]));
                if(result.has_value()){
                    view_block(result.value());
                } else {
                    std::cout << "Aucun block ne correspond a cet index !" << std::endl;
                }
            } else if (user_command_part[0] == "/find_block_by_hash") {
                // /find_block_by_hash [hash]
                std::optional<Block> result = find_block_by_hash(blocks, user_command_part[1]);
                if(result.has_value()){
                    view_block(result.value());
                } else {
                    std::cout << "Aucun block ne correspond a cet index !" << std::endl;
                }
            } else if (user_command_part[0] == "/balance") {
                // /balance
                std::cout << "-> Balance : " << satochi_to_btc(currentUser.balance) << "BTC \n" << std::endl;
                std::cout << "-> UTXOs : \n";
                for(UTXO utxo : currentUser.utxos){
                    std::cout << "-> txid: " << utxo.txid_transaction << ", vout: " << utxo.vout << ", amount: " << utxo.amount << "\n";
                }
                
            } else if (user_command_part[0] == "/swipe"){
                // /swipe [current_user_publicKey] [next_user_publicKey]
                if(user_command_part[1] != currentUser.publicKey){
                    std::cout << "La clé de votre compte courant est erroné ! \n" << std::endl;
                    continue; 
                }
                
                std::optional<User> match = find_user_by_public_key(user_command_part[2], users);

                if(match.has_value()) {
                    currentUser = match.value();
                    std::cout << "Vous avez switch correctement entre deux comptes ! \n" << std::endl;
                    sync_user(users, currentUser);
                } else {
                    std::cout << "Veuillez vérifier que la clé publique du compte de destination est correcte ! \n" << std::endl;
                    continue;
                } 
            } 
            
            
            else if (user_command_part[0] == "/help") {
                help();
                continue;
            } else if (user_command_part[0] == "/d"){
                currentUser = User("", "", "", 0.0, {});
                std::cout << "Vous avez été déconnecté !";
                return 0;
            } else if (user_command == "/q"){
                std::cout << "A bientot !" << std::endl;
                return false;
            } else {
                std::cout << "La commande " << user_command_part[0] << " n'existe pas !" << std::endl;
                continue;
            }







        } else { // Commandes qui ne demandent pas d'etre connecté a un compte 
            if (user_command_part[0] == "/c"){
                // /c [publicKey]

                std::optional<User> match = find_user_by_public_key(user_command_part[1], users);
                
                if(match.has_value()){
                    currentUser = match.value();
                    std::cout << "Vous êtes connecté a l'utilisateur : \n" << std::endl;
                    std::cout << "privateKey : " << currentUser.privateKey << "\n" << std::endl;
                    std::cout << "publicKey : " << currentUser.publicKey << "\n" << std::endl;
                    std::cout << "address : " << currentUser.address << "\n" << std::endl;
                    std::cout << "balance : " << currentUser.balance << "\n" << std::endl;

                }else {
                    std::cout << "Cette addresse n'est attribué a aucun utilisateur !" << std::endl;
                    continue;
                }
            } else if (user_command_part[0] == "/users") {
                for(User user : users) {
                    std::cout << "→ adresse: " << user.address << ", balance: " << user.balance << ", publicKey: " << user.publicKey << "\n";
                }
            } else if (user_command_part[0] == "/mempool") {
                if(mempool.transactions.empty()){
                    std::cout << "Le mempool est vide !" << std::endl;
                } else {
                    std::cout << mempool.transactions.size() << " transaction(s) en attente\n" << std::endl;
                    for(Transaction item : mempool.transactions) {
                        std::cout << "txid    : " << item.txid << "\n";
                        std::cout << "montant : " << satochi_to_btc(item.value_btc) << " BTC\n";
                        std::cout << "fee     : " << item.fee << " sats\n";
                        std::cout << "inputs  : " << item.nb_input << "\n";
                        std::cout << "outputs : " << item.nb_output << "\n";
                        std::cout << "de      : ";
                        for(std::string src : item.adresses_sources){
                            std::cout << src << " ";
                        }
                        std::cout << "\n";
                        std::cout << "vers    : ";
                        for(std::string dest : item.adresses_dest){
                            std::cout << dest << " ";
                        }
                        std::cout << "\n---\n";
                    }
                }
            } 
            
            
            
            else if (user_command_part[0] == "/help") {
                help();
                continue;
            } else if (user_command == "/q"){
                std::cout << "A bientot !" << std::endl;
                return false;
            } else {
                std::cout << "La commande " << user_command_part[0] << " n'existe pas !" << std::endl;
                continue;
            }
        };
    };
}