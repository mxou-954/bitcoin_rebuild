#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "entities.h"

using namespace std;

Block new_block(int index,
    std::string hash,
    std::string prevHash,
    int amount,
    time_t timestamp,
    int nonce,
    int difficulty,
    std::vector<Transaction> transactions,
    bool resolved = false);

#endif
