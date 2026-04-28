#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "entities.h"

using namespace std;

std::optional<Block> find_block_by_index(std::vector<Block> blocks, int index);
std::optional<Block> find_block_by_hash(std::vector<Block> blocks, std::string hash);
Block new_block(std::vector<Block>& blockchain, std::vector<Transaction> transactions);
bool mine(std::vector<Block>& blockchain, int nonce, Mempool mempool);
Block view_block(Block block);

#endif
