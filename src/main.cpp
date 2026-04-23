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

#include <openssl/sha.h>

using namespace std;

int main() {
    srand(time(0));
    std::vector<User> users;


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
        User currentUser = User("", "", "", 0.0);
        std::string user_command = "";
        std::string delimiter = " ";

        std::cout << "\n core>> ";
        std::getline(std::cin, user_command);
        if(user_command == "/q"){
            std::cout << "A bientot !" << std::endl;
            return false;
        }

        std::vector<std::string> user_command_part = split(user_command, delimiter);
        
        // for(int i =0; i < user_command_part.size(); i++){
        //     std::cout << user_command_part[i] << std::endl; // Voir la commande utilisateur divisé
        // }

        if(user_command_part[0] == "/pay") {
            // /pay [amount] [address]
            time_t timestamp;
            double vsize = 160.0; // vbytes
            double fee_per_vbyte = 10.0; // sat/vbytes
            std::vector<std::string> adresses_sources;
            std::vector<std::string> adresses_dest;

            adresses_sources.push_back("btc1xxxxxxxxxxxxx");
            adresses_dest.push_back(user_command_part[2]);
            
            Transaction new_transaction = Transaction(
                generate_random_number(50), 
                time(&timestamp),
                btc_to_satochi(stod(user_command_part[1])),
                vsize * fee_per_vbyte,
                vsize, 
                fee_per_vbyte,
                0,
                0,
                0,
                adresses_sources,
                adresses_dest
            );
        } else if (user_command_part[0] == "/c"){
            // /c [publicKey]
            
            for(User u : users){
                if(u.publicKey == user_command_part[1]){
                    currentUser = u;
                }
            }

            std::cout << "Vous êtes connecté a l'utilisateur : \n" << std::endl;
            std::cout << "privateKey : " << currentUser.privateKey << "\n" << std::endl;
            std::cout << "publicKey : " << currentUser.publicKey << "\n" << std::endl;
            std::cout << "address : " << currentUser.address << "\n" << std::endl;
            std::cout << "balance : " << currentUser.balance << "\n" << std::endl;
        } else if (user_command_part[0] == "/d"){
            currentUser = User("", "", "", 0.0);
            std::cout << "Vous avez été déconnecté !";
        } else if (user_command_part[0] == "/swipe"){
            // /swipe [current_user_publicKey] [next_user_publicKey]
            if(user_command_part[1] != currentUser.publicKey){
                std::cout << "La clé de votre compte courant est erroné ! \n" << std::endl;
                continue; 
            }
            bool match = false; 
            for(User u : users){
                if(u.publicKey == user_command_part[2]){
                    match = true;
                    currentUser = u;
                }
            }

            if(match == true) {
                std::cout << "Vous avez switch correctement entre deux comptes ! \n" << std::endl;
            } else {
                std::cout << "Veuillez vérifier que la clé publique du compte de destination est correcte ! \n" << std::endl;
            }
        }

    };
}