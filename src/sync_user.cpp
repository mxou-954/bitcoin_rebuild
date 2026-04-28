#include <iostream>
#include <string>
#include <vector>

#include "entities.h"

void sync_user(std::vector<User>& users, User& currentUser){
    for(User& u : users){
        if(u.address == currentUser.address){
            u = currentUser;
            break;
        }
    }
}