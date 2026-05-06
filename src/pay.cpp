#include "utxos.h"
#include "find_user.h"
#include "sha256.h"
#include "security_check_for_pay.h"
#include "sync_user.h"
#include "write_and_read.h"

#include <iostream>
#include <string>
#include <optional>
#include <vector>

#include "entities.h"
#include "btc_to_satochi.h"

void pay(std::vector<std::string> user_command_part, std::vector<User> users, User currentUser, Mempool mempool) {
    std::optional<User> match_address = find_user_by_address(user_command_part[3], users);
    std::optional<User> match_public_key = find_user_by_public_key(user_command_part[1], users);

    if(!security_check_for_pay(currentUser, match_address, match_public_key, user_command_part)){
        return;
    }

    double addition = 0.0;
    std::vector<UTXO> to_use;
    std::vector<UTXO> currentUser_utxos = currentUser.utxos;
    while(addition < stod(user_command_part[2]) && !currentUser_utxos.empty()){
        UTXO utxo = currentUser_utxos.back();
        addition = addition + utxo.amount;
        to_use.push_back(utxo);
        currentUser_utxos.pop_back();
    }

    time_t timestamp = time(nullptr);
    currentUser.utxos = currentUser_utxos;
    double amount_output = addition - stod(user_command_part[2]);

    std::stringstream ss;
    ss << timestamp << currentUser.address << user_command_part[3] << user_command_part[2];
    std::string txid = sha256(ss.str());

    // Changer le generate number par le vrai id de la transaction
    UTXO to_sender = create_utxo(currentUser, amount_output, txid, 0); 
    UTXO to_receiver = create_utxo(match_address.value(), stod(user_command_part[2]), txid, 1);

    std::vector<UTXO> to_return;
    to_return.push_back(to_sender);
    to_return.push_back(to_receiver);

    double vsize = 160.0; // vbytes
    double fee_per_vbyte = 10.0; // sat/vbytes
    std::vector<std::string> adresses_sources;
    std::vector<std::string> adresses_dest;

    adresses_sources.push_back(currentUser.address);
    adresses_dest.push_back(user_command_part[2]);
    
    Transaction new_transaction = Transaction(
        txid,
        time(&timestamp),
        btc_to_satochi(stod(user_command_part[2])),
        vsize * fee_per_vbyte,
        vsize,
        fee_per_vbyte,
        to_use.size(),
        0,
        0,
        adresses_sources,
        adresses_dest,
        to_use,
        to_return
    );

    match_address.value().utxos.push_back(to_receiver);
    currentUser.utxos.push_back(to_sender);

    match_address.value().balance = utxos_sum_balance(match_address.value());
    currentUser.balance = utxos_sum_balance(currentUser);

    sync_user(users, currentUser);
    sync_user(users, match_address.value());

    if (mempool.transactions.size() < mempool.max_size){
        mempool.transactions.push_back(new_transaction);
    }

    export_user_in_registre("../registre/users.json", users);
    export_mempool_in_registre("../registre/mempool.json", mempool);
                
}