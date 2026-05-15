#include "utxos.h"
#include "generate_random_number.h"
#include "btc_to_satochi.h"
#include "sha256.h"
#include "sync_user.h"
#include "write_and_read.h"
#include "transfer_eur_to_btc.h"

#include <iostream>
#include <string>
#include <variant>

std::variant<bool, std::string> transfer_eur_to_btc(
    std::vector<std::string> user_command_part, 
    User& currentUser, 
    std::vector<User>& users
) {
    
    if(sha256(currentUser.privateKey) != user_command_part[2]){
        return std::string("La clé publique n'est pas celle liée à votre compte.");
    }

    double euros = stod(user_command_part[1]);
    double btc_price = 85000.0;
    double montant_btc = euros / btc_price;
    double montant_satoshis = btc_to_satochi(montant_btc);

    UTXO new_utxo = create_utxo(currentUser, montant_satoshis, generate_random_number(50), 0);
    
    currentUser.utxos.push_back(new_utxo);
    currentUser.balance += montant_satoshis;

    sync_user(users, currentUser);
    export_user_in_registre("./registre/users.json", users);

    return true;
}