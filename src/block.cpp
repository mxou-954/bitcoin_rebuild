#include "sha256.h"
#include "entities.h"

Block new_block(int index,
    std::string hash,
    std::string prevHash,
    int amount,
    time_t timestamp,
    int nonce,
    int difficulty,
    std::vector<Transaction> transactions,
    bool resolved = false) {

    Block block = Block {
        index,
        prevHash, 
        amount, 
        timestamp, 
        nonce, 
        difficulty, 
        transactions, 
        resolved
    };
}