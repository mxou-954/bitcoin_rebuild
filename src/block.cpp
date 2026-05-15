#include "sha256.h"
#include "entities.h"
#include "generate_random_number.h"
#include "write_and_read.h"
#include "utxos.h"
#include "sync_user.h"

#include <sstream>
#include <string>
#include <iostream>
#include <optional>
#include <ctime>

Block genesis_block() {
    time_t timestamp;
    Block genesis;
    genesis.index     = 0;
    genesis.prevHash  = "0000000000000000";
    genesis.timestamp = time(&timestamp);
    genesis.difficulty = 2;
    genesis.nonce     = 0;
    genesis.transactions = {};
    genesis.hash      = generate_hash_with_zeros(2);
    return genesis;
}

std::optional<Block> find_block_by_index(std::vector<Block> blocks, int index) {
    for(Block block : blocks) {
        if(block.index == index){
            return block;
        } 
    }
    std::cout << "Block introuvable !" << std::endl;
    return std::nullopt;
}

std::optional<Block> find_block_by_hash(std::vector<Block> blocks, std::string hash) {
    for(Block block : blocks) {
        if(block.hash == hash){
            return block;
        } 
    }
    std::cout << "Block introuvable !" << std::endl;
    return std::nullopt;
}

Block new_block(std::vector<Block>& blockchain, 
                std::vector<Transaction>& transactions) {
    
    Block last = blockchain.back();

    time_t timestamp;
    int difficulty = 2;
    
    Block block;
    block.index        = last.index + 1;
    block.prevHash     = last.hash;
    block.timestamp    = time(&timestamp);
    block.difficulty   = difficulty;
    block.transactions = transactions;
    block.nonce        = 0;

    blockchain.push_back(block);
    
    return block;
}

bool mine(std::vector<Block>& blockchain, int nonce, Mempool& mempool, User& currentUser, std::vector<User>& users) {
    if(blockchain.empty()){
        blockchain.push_back(genesis_block());
        new_block(blockchain, mempool.transactions);
    }
    
    Block& block = blockchain.back();
    if(block.resolved){
        std::cout << "Ce bloc est déjà résolu !" << std::endl;
        return false;
    }
    
    int difficulty = block.difficulty;
    std::string target(difficulty, '0');
    
    std::stringstream ss;
    ss << block.index
       << block.prevHash
       << block.timestamp
       << nonce;
    
    std::string computed_hash = sha256(ss.str());
    
    if(computed_hash.substr(0, difficulty) != target){
        std::cout << "Hash invalide !" << std::endl;
        return false;
    }
    
    block.nonce    = nonce;
    block.hash     = computed_hash;
    block.resolved = true;

    export_blockchain_in_registre("./registre/blockchain.json", blockchain);
    
    std::cout << "Bloc miné ! nonce=" << nonce << std::endl;
    std::cout << "hash=" << computed_hash << std::endl;

    double fee_sum = 0.0; //sat récompense mineur (block reward + somme fees)
    double block_reward = 50000.0;
    for(Transaction item : mempool.transactions) {
        fee_sum += item.fee;
    }
    double reward_sum = fee_sum + block_reward;

    UTXO utxo_reward = create_utxo(currentUser, reward_sum, block.hash, 0);
    std::cout << "Vous avez miné le block, vous recevez : " << reward_sum << " satochis\n";
    currentUser.utxos.push_back(utxo_reward);

    currentUser.balance = utxos_sum_balance(currentUser);
    sync_user(users, currentUser);
    export_user_in_registre("./registre/users.json", users);

    new_block(blockchain, mempool.transactions);
    mempool.transactions.clear();
    
    return true;
}

bool auto_mining(std::vector<Block>& blockchain, Mempool& mempool, User& currentUser, std::vector<User>& users) {
    if(blockchain.empty()){
        blockchain.push_back(genesis_block());
        new_block(blockchain, mempool.transactions);
    }

    Block& block = blockchain.back();
    if(block.resolved){
        std::cout << "Ce bloc est déjà résolu !" << std::endl;
        return false;
    }

    std::string target(block.difficulty, '0');
    int nonce = 0;
    std::string computed_hash = std::string(block.difficulty, '1');

    while(computed_hash.substr(0, block.difficulty) != target){
        
       std::stringstream ss;
        ss << block.index
        << block.prevHash
        << block.timestamp
        << nonce;
    
        computed_hash = sha256(ss.str());

        nonce++;
        std::cout << "Test du nonce : " << nonce << std::endl;
    }

    block.nonce    = nonce;
    block.hash     = computed_hash;
    block.resolved = true;

    export_blockchain_in_registre("./registre/blockchain.json", blockchain);
    
    std::cout << "Bloc miné ! nonce=" << nonce << std::endl;
    std::cout << "hash=" << computed_hash << std::endl;

    double fee_sum = 0.0; //sat récompense mineur (block reward + somme fees)
    double block_reward = 50000.0;
    for(Transaction item : mempool.transactions) {
        fee_sum += item.fee;
    }
    double reward_sum = fee_sum + block_reward;

    UTXO utxo_reward = create_utxo(currentUser, reward_sum, block.hash, 0);
    std::cout << "Vous avez miné le block, vous recevez : " << reward_sum << " satochis\n";
    currentUser.utxos.push_back(utxo_reward);

    currentUser.balance = utxos_sum_balance(currentUser);
    sync_user(users, currentUser);
    export_user_in_registre("./registre/users.json", users);

    new_block(blockchain, mempool.transactions);
    mempool.transactions.clear();
    
    return true;
}

void view_block(Block block) {
    std::cout << "==========================================\n";
    std::cout << "index      : " << block.index << "\n";
    std::cout << "hash       : " << block.hash << "\n";
    std::cout << "prevHash   : " << block.prevHash << "\n";
    std::cout << "timestamp  : " << block.timestamp << "\n";
    std::cout << "nonce      : " << block.nonce << "\n";
    std::cout << "difficulty : " << block.difficulty << "\n";
    std::cout << "resolved   : " << (block.resolved ? "oui" : "non") << "\n";
    std::cout << "transactions : " << block.transactions.size() << " tx\n";
    for(Transaction t : block.transactions) {
        std::cout << "  → txid : " << t.txid << "\n";
        std::cout << "     montant : " << t.value_btc << " BTC\n";
    }
    std::cout << "==========================================\n";
}

void find_block_by_hash_output(std::vector<Block> blocks, std::vector<std::string> user_command_part){
    std::optional<Block> result = find_block_by_hash(blocks, user_command_part[1]);
    if(result.has_value()){
        view_block(result.value());
    } else {
        std::cout << "Aucun block ne correspond a cet index !" << std::endl;
    }
}

void find_block_by_index_output(std::vector<Block> blocks, std::vector<std::string> user_command_part){
    std::optional<Block> result = find_block_by_index(blocks, stoi(user_command_part[1]));
    if(result.has_value()){
        view_block(result.value());
    } else {
        std::cout << "Aucun block ne correspond a cet index !" << std::endl;
    }
}