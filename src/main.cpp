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

#include <openssl/sha.h>

using namespace std;

int main() {
    srand(time(0));
    std::vector<User> users;
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
        if(user_command == "/q"){
            std::cout << "A bientot !" << std::endl;
            return false;
        }

        std::vector<std::string> user_command_part = split(user_command, delimiter);


        // Commandes qui demandent d'etre connecté a un compte 
        if(is_connected){
            if(user_command_part[0] == "/pay") {
                // /pay [publicKey] [amount] [address]

                std::optional<User> match_address = find_user_by_address(user_command_part[3], users);
                std::optional<User> match_public_key = find_user_by_public_key(user_command_part[1], users);
                if(!match_address.has_value()){
                    std::cout << "L'adresse de destination n'appartient a aucun utilisateur !" << std::endl;
                    continue;
                }
                if(!match_public_key.has_value()){
                    std::cout << "La clé public ne correspond pas a un utilisateur valide !" << std::endl;
                    continue;
                }
                if(sha256(currentUser.privateKey) != user_command_part[1]){
                    std::cout << "La clé public n'est pas celle lié a votre compte." << std::endl;
                    continue;
                }
                if(stod(user_command_part[2]) > currentUser.balance){
                    std::cout << "Vous n'avez pas les fonds nécessaire pour le payement !" << std::endl;
                    continue;
                }

                double addition = 0.0;
                std::vector<UTXO> to_use;
                std::vector<UTXO> currentUser_utxos = currentUser.utxos;
                while(addition < stod(user_command_part[2])){
                    for(UTXO utxo: currentUser_utxos) {

                        if(to_use.size() == currentUser_utxos.size()){
                            std::cout << "Fonds insuffisants !" << std::endl;
                            break;
                        }

                        addition = addition + utxo.amount;
                        to_use.push_back(utxo);
                    }
                }

                time_t timestamp;
                double vsize = 160.0; // vbytes
                double fee_per_vbyte = 10.0; // sat/vbytes
                std::vector<std::string> adresses_sources;
                std::vector<std::string> adresses_dest;

                adresses_sources.push_back(currentUser.address);
                adresses_dest.push_back(user_command_part[2]);
                
                Transaction new_transaction = Transaction(
                    generate_random_number(50), 
                    time(&timestamp),
                    btc_to_satochi(stod(user_command_part[2])),
                    vsize * fee_per_vbyte,
                    vsize, 
                    fee_per_vbyte,
                    to_use.size(),
                    0,
                    0,
                    adresses_sources,
                    adresses_dest
                );




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
                } else {
                    std::cout << "Veuillez vérifier que la clé publique du compte de destination est correcte ! \n" << std::endl;
                    continue;
                } 
            } else if (user_command_part[0] == "/d"){
                currentUser = User("", "", "", 0.0, {});
                std::cout << "Vous avez été déconnecté !";
                continue;
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
            } else {
                std::cout << "La commande " << user_command_part[0] << " n'existe pas !" << std::endl;
                continue;
            }
        };
    };
}