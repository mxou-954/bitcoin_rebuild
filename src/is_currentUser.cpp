#include <iostream>
#include <string>

#include "is_currentUser.h"

bool is_currentUser(User& currentUser){
    if(currentUser.privateKey == ""){
        return false;
    } else {
        return true;
    }
}