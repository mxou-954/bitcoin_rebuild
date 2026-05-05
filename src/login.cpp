#include "login.h"
#include "find_user.h"

#include <string>
#include <iostream>

void login(std::vector<std::string> user_command_part, std::vector<User> users, User& currentUser){
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
        return;
    }
}