#ifndef PAY_H
#define PAY_H

#include <string>
#include <vector>
#include "entities.h"

using namespace std;

void pay(std::vector<std::string> user_command_part, std::vector<User> users, User currentUser, Mempool mempool);

#endif
