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
}

std::optional<UTXO> find_utxo_by_txid_transaction(User user, std::string txid_transaction){
    std::vector<UTXO> user_utxos = user.utxos;
    for(UTXO utxo : user_utxos){
        if(txid_transaction == utxo.txid_transaction){
            return utxo;
        }
    }
}
