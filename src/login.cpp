#include "login.h"
#include "find_user.h"
#include "btc_to_satochi.h"

#include <string>
#include <iostream>
#include <iomanip>

void login(std::vector<std::string> user_command_part, std::vector<User>& users, User& currentUser){
    User* match = find_user_by_public_key(user_command_part[1], users);
                
    if(match){
        currentUser = *match;
        std::cout << "Vous êtes connecté a l'utilisateur : \n" << std::endl;
        std::cout << "privateKey : " << currentUser.privateKey << "\n" << std::endl;
        std::cout << "publicKey : " << currentUser.publicKey << "\n" << std::endl;
        std::cout << "address : " << currentUser.address << "\n" << std::endl;
        std::cout << "balance : " << std::fixed << std::setprecision(6) << satochi_to_btc(currentUser.balance) / 1e8 << " BTC" << std::endl;
    }else {
        std::cout << "Cette addresse n'est attribué a aucun utilisateur !" << std::endl;
        return;
    }
}