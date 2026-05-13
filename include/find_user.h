#ifndef FIND_USER_H
#define FIND_USER_H

#include <string>
#include <vector>
#include "entities.h"
#include <optional>

#include "entities.h"

User* find_user_by_public_key(std::string publicKey, std::vector<User>& users);
User* find_user_by_address(std::string address, std::vector<User>& users);

#endif