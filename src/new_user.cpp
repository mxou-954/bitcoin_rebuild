#include "new_user.h"
#include "generate_random_number.h"
#include "sha256.h"

User new_user() {
    std::string prv_key = generate_random_number(50);
    std::string pblc_key = sha256(prv_key);
    std::string address = "btc1" + generate_random_number(50);
    
    User new_user = User(prv_key, pblc_key, address, 3.0, {});

    return new_user;
}