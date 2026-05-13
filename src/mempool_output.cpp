#include <string>
#include <iostream>

#include "btc_to_satochi.h"
#include "mempool_output.h"

void mempool_output(std::vector<Transaction>& mempool_transactions) {
    if(mempool_transactions.empty()){
        std::cout << "Le mempool est vide !" << std::endl;
        return;
    } 

    double fee_sum = 0.0; //sat récompense mineur (block reward + somme fees)
    double block_reward = 50000.0; // récompense en sat simulé (constante) a mettre dans le fichier de constantes 

    std::cout << mempool_transactions.size() << " transaction(s) en attente\n" << std::endl;
    for(Transaction item : mempool_transactions) {
        std::cout << "txid     : " << item.txid << "\n";
        std::cout << "montant  : " << satochi_to_btc(item.value_btc) << " BTC\n";
        std::cout << "fee      : " << item.fee << " sats\n";
        std::cout << "fee_rate : " << item.fee_per_vbyte << " sat/vbytes\n";
        std::cout << "vsize    : " << item.vsize << " vbytes\n";
        std::cout << "inputs   : " << item.nb_input << "\n";
        std::cout << "outputs  : " << item.nb_output << "\n";
        std::cout << "de       : ";
        for(std::string src : item.adresses_sources){
            std::cout << src << " ";
        }
        std::cout << "\n";
        std::cout << "vers     : ";
        for(std::string dest : item.adresses_dest){
            std::cout << dest << " ";
        }
        std::cout << "\n---\n";
        fee_sum += item.fee;
    }
    std::cout << "Récompences du mineur : " << fee_sum + block_reward << "\n";
}