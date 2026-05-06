#ifndef SECURITY_CHECK_FOR_PAY_H
#define SECURITY_CHECK_FOR_PAY_H

#include <string>
#include "entities.h"
#include <optional>
#include <iostream>

using namespace std;

bool security_check_for_pay(User currentUser, std::optional<User> match_address, 
                             std::optional<User> match_public_key, 
                             std::vector<std::string> user_command_part);
                             
#endif