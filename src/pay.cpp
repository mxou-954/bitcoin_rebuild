#include "utxos.h"
#include "find_user.h"
#include "sha256.h"
#include "security_check_for_pay.h"
#include "sync_user.h"
#include "write_and_read.h"
#include "btc_to_satochi.h"

#include <iostream>
#include <string>
#include <optional>
#include <vector>

#include "entities.h"
#include "btc_to_satochi.h"

void pay(std::vector<std::string> user_command_part, std::vector<User>& users, User& currentUser, Mempool& mempool) {
    User* match_address = find_user_by_address(user_command_part[3], users);
    User* match_public_key = find_user_by_public_key(user_command_part[1], users);

    if(match_address == nullptr || match_public_key == nullptr){
        std::cout << "Utilisateur introuvable." << std::endl;
        return;
    }

    if(!security_check_for_pay(currentUser, match_address, match_public_key, user_command_part)){
        std::cout << "Une erreur a été détectée..." << std::endl;
        return;
    }

    double addition = 0.0;
    std::vector<UTXO> to_use;
    std::vector<UTXO> currentUser_utxos = currentUser.utxos;
    while(addition < btc_to_satochi(stod(user_command_part[2])) && !currentUser_utxos.empty()){ // mettre amount en sat
        UTXO utxo = currentUser_utxos.back();
        addition = addition + utxo.amount;
        to_use.push_back(utxo);
        currentUser_utxos.pop_back();
    }

    time_t timestamp = time(nullptr);
    currentUser.utxos = currentUser_utxos;

    std::stringstream ss;
    ss << timestamp << currentUser.address << user_command_part[3] << user_command_part[2];
    std::string txid = sha256(ss.str());

    double nb_outputs = 2.0;
    double vsize = 10.0 + (to_use.size() * 148.0) + (nb_outputs * 34.0);
    double fee_per_vbyte = 10.0; // sat/vbyte fixe
    double fees = vsize * fee_per_vbyte;
    double amount_output = addition - btc_to_satochi(stod(user_command_part[2])) - fees;

    // Changer le generate number par le vrai id de la transaction
    UTXO to_sender = create_utxo(currentUser, amount_output, txid, 0); 
    UTXO to_receiver = create_utxo(*match_address, btc_to_satochi(stod(user_command_part[2])), txid, 1);

    std::vector<UTXO> to_return;
    to_return.push_back(to_sender);
    to_return.push_back(to_receiver);

    
    std::vector<std::string> adresses_sources;
    std::vector<std::string> adresses_dest;

    adresses_sources.push_back(currentUser.address);
    adresses_dest.push_back(user_command_part[3]);
        
    Transaction new_transaction = Transaction(
        txid,
        timestamp,
        btc_to_satochi(stod(user_command_part[2])),
        vsize * fee_per_vbyte,
        vsize,
        fee_per_vbyte,
        to_use.size(),
        to_return.size(),
        1,
        adresses_sources,
        adresses_dest,
        to_use,
        to_return
    );

    match_address->utxos.push_back(to_receiver);
    currentUser.utxos.push_back(to_sender);

    match_address->balance = utxos_sum_balance(*match_address);
    currentUser.balance = utxos_sum_balance(currentUser);

    sync_user(users, currentUser);

    if (mempool.transactions.size() < mempool.max_size){
        mempool.transactions.push_back(new_transaction);
    }

    export_user_in_registre("./registre/users.json", users);
    export_mempool_in_registre("./registre/mempool.json", mempool);

    std::cout << "Le payement a été effectué !" << std::endl;
                
}