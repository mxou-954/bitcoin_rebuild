#include "new_user.h"
#include "generate_random_number.h"
#include "write_and_read.h"
#include "sha256.h"

User new_user(std::vector<User>& users) {
    std::string prv_key = generate_random_number(50);
    std::string pblc_key = sha256(prv_key);
    std::string address = "btc1" + generate_random_number(50);
    
    User new_user = User(prv_key, pblc_key, address, 0.0, {});

    export_user_in_registre("./registre/users.json", users);

    return new_user;
}