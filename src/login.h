#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <vector>
#include "entities.h"

void login(std::vector<std::string> user_command_part, std::vector<User> users, User& currentUser);

#endif