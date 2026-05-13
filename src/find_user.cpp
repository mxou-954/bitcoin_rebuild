#include "find_user.h"
#include <iostream>
#include <string>
#include <optional>

User* find_user_by_public_key(std::string publicKey, std::vector<User>& users){
    for(User& u : users){
        if(u.publicKey == publicKey){
            return &u;
        }
    }
    std::cout << "Utilisateur introuvable !" << std::endl;
    return nullptr;
}

User* find_user_by_address(std::string address, std::vector<User>& users){
    for(User& u : users){
        if(u.address == address){
            return &u;
        }
    }
    std::cout << "Utilisateur introuvable !" << std::endl;
    return nullptr;
}