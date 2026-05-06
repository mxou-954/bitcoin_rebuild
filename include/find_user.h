#ifndef FIND_USER_H
#define FIND_USER_H

#include <string>
#include <vector>
#include "entities.h"
#include <optional>

#include "entities.h"

std::optional<User> find_user_by_public_key(std::string publicKey, std::vector<User> users);
std::optional<User> find_user_by_address(std::string address, std::vector<User> users);

#endif