#include "swipe_user.h"
#include "find_user.h"
#include "sync_user.h"

#include <iostream>
#include <string>
#include <optional>
#include <vector>

#include "entities.h"

void swipe_user(User currentUser, std::vector<std::string> user_command_part, std::vector<User> users){
    if(user_command_part[1] != currentUser.publicKey){
        std::cout << "La clé de votre compte courant est erroné ! \n" << std::endl;
        return; 
    }
    
    std::optional<User> match = find_user_by_public_key(user_command_part[2], users);

    if(match.has_value()) {
        currentUser = match.value();
        std::cout << "Vous avez switch correctement entre deux comptes ! \n" << std::endl;
        sync_user(users, currentUser);
    } else {
        std::cout << "Veuillez vérifier que la clé publique du compte de destination est correcte ! \n" << std::endl;
        return;
    } 
}