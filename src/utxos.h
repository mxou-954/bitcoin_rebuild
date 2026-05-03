#ifndef UTXOS_H
#define UTXOS_H

#include <string>
#include "entities.h"
#include <optional>

UTXO create_utxo(User user, double amount, std::string txid_transaction, int vout);
std::optional<UTXO> find_utxo_by_txid_transaction(User user, std::string txid_transaction);
double utxos_sum_balance(User user);

#endif
