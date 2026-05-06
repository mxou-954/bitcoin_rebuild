#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "entities.h"

using namespace std;

Block genesis_block();
std::optional<Block> find_block_by_index(std::vector<Block> blocks, int index);
std::optional<Block> find_block_by_hash(std::vector<Block> blocks, std::string hash);
Block new_block(std::vector<Block>& blockchain, std::vector<Transaction> transactions);
bool mine(std::vector<Block>& blockchain, int nonce, Mempool& mempool);
void view_block(Block block);
void find_block_by_hash_output(std::vector<Block> blocks, std::vector<std::string> user_command_part);
void find_block_by_index_output(std::vector<Block> blocks, std::vector<std::string> user_command_part);

#endif
