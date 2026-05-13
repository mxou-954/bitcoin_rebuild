#ifndef TRANSFER_EUR_TO_BTC_H
#define TRANSFER_EUR_TO_BTC_H

#include <string>
#include <vector>
#include "entities.h"
#include <variant>

std::variant<bool, std::string> transfer_eur_to_btc(
    std::vector<std::string> user_command_part, 
    User& currentUser, 
    std::vector<User>& users
);

#endif