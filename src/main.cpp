#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>

#include "include/entities.h"
#include "include/generate_random_number.h"
#include "include/sha256.h"
#include "include/new_user.h"
#include "include/split.h"
#include "include/btc_to_satochi.h" 
#include "include/is_currentUser.h"
#include "include/find_user.h"
#include "include/utxos.h"
#include "include/block.h"
#include "include/sync_user.h"
#include "include/help.h"
#include "include/write_and_read.h"
#include "include/security_check_for_pay.h"
#include "include/user_input.h"
#include "include/transfer_eur_to_btc.h"
#include "include/mempool_output.h"
#include "include/login.h"
#include "include/swipe_user.h"
#include "include/transfer.h"
#include "include/pay.h"

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

        // Gestion de l'input utilisateur //
        std::string delimiter = " ";
        std::vector<std::string> user_command_part = user_input(currentUser, delimiter);
        ////////////////////////////

        // NECESSITE D'ETRE CONNECTE 
        if(is_connected){
            if(user_command_part[0] == "/pay") {
                // /pay [publicKey] [amount] [address]
                pay(user_command_part, users, currentUser, mempool);
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/transfer") {
                // /transfer [euros] [publicKey]
                transfer(user_command_part, currentUser, users);
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/find_block_by_index") {
                // /find_block_by_index [index]
                find_block_by_index_output(blocks, user_command_part);
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/find_block_by_hash") {
                // /find_block_by_hash [hash]
                find_block_by_hash_output(blocks, user_command_part);
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/balance") {
                // /balance
                // FONCTION D'AFFICHAGE //
                balance(currentUser);
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/swipe"){
                // /swipe [current_user_publicKey] [next_user_publicKey]
                swipe_user(currentUser, user_command_part, users);
                /////////////////////////////
            } else if (user_command_part[0] == "/mine") {
                // /mine [nonce]
                bool result = mine(blocks, stoi(user_command_part[1]), mempool);
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/blockchain") {
                for(const Block& block : blocks){
                    view_block(block);
                }
                continue;
            }else if (user_command_part[0] == "/help") {
                // FONCTION D'AFFICHAGE //
                help();
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/d"){
                currentUser = User("", "", "", 0.0, {});
                std::cout << "Vous avez été déconnecté !";
                continue;
            } else if (user_command_part[0] == "/q"){
                std::cout << "A bientot !" << std::endl;
                return false;
            } else {
                std::cout << "La commande " << user_command_part[0] << " n'existe pas !" << std::endl;
                continue;
            }
        } else { // Commandes qui ne demandent pas d'etre connecté a un compte 
            if (user_command_part[0] == "/c"){
                // /c [publicKey]
                login(user_command_part, users, currentUser);
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/users") {
                for(User user : users) {
                    std::cout << "-> adresse: " << user.address << ", balance: " << user.balance << ", publicKey: " << user.publicKey << "\n";
                }
                continue;
            } else if (user_command_part[0] == "/mempool") {
                // FONCTION D'AFFICHAGE //
                mempool_output(mempool.transactions);
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/help") {
                // FONCTION D'AFFICHAGE //
                help();
                continue;
                /////////////////////////////
            } else if (user_command_part[0] == "/q"){
                std::cout << "A bientot !" << std::endl;
                return false;
            } else {
                std::cout << "La commande " << user_command_part[0] << " n'existe pas !" << std::endl;
                continue;
            }
        };
    };
}