#include "transfer_eur_to_btc.h"
#include "entities.h"

#include <string>
#include <iostream>

void transfer(
    std::vector<std::string> user_command_part, 
    User& currentUser, 
    std::vector<User>& users
){
    auto result = transfer_eur_to_btc(user_command_part, currentUser, users);

    if (std::holds_alternative<std::string>(result)) {
        std::cout << std::get<std::string>(result) << std::endl;
    } else {
        bool success = std::get<bool>(result);
        std::cout << "Transfert réussi !" << std::endl;
    }
}