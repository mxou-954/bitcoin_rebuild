#include <iostream>

#include "user_input.h"
#include "split.h"

std::vector<std::string> user_input(User currentUser, std::string delimiter) {
    std::string user_command = "";

    if (currentUser.address == ""){
        std::cout << "\n" << "guest >> ";
    } else {
        std::cout << "\n" << currentUser.address << " >> ";
    }
    
    std::getline(std::cin, user_command);
    std::vector<std::string> user_command_part = split(user_command, delimiter);
    return user_command_part;
}