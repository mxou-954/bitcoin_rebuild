#include "utxos.h"

#include <iostream>
#include <string>
#include <optional>
#include <vector>

#include "entities.h"

UTXO create_utxo(User user, double amount, std::string txid_transaction, int vout){
    UTXO new_utxo = UTXO(
        amount, 
        user.publicKey,
        txid_transaction,
        vout
    );
    return new_utxo;
}

std::optional<UTXO> find_utxo_by_txid_transaction(User user, std::string txid_transaction){
    std::vector<UTXO> user_utxos = user.utxos;
    for(UTXO utxo : user_utxos){
        if(txid_transaction == utxo.txid_transaction){
            return utxo;
        }
    }
    return std::nullopt;
}

double utxos_sum_balance(User user){
    double result = 0;
    for(UTXO utxo : user.utxos){
        result = result + utxo.amount;
    }
    return result;
}
