#include "sha256.h"
#include "entities.h"
#include "generate_random_number.h"

#include <iostream>
#include <string>
#include <optional>
#include <ctime>

std::optional<Block> find_block_by_index(std::vector<Block> blocks, int index) {
    for(Block block : blocks) {
        if(block.index == index){
            return block;
        } 
    }
    std::cout << "Block introuvable !" << std::endl;
    return std::nullopt;
}

Block new_block(std::vector<Block> blocks){
    srand(time(0));
    time_t timestamp;
    Block last_block = blocks.back();
    std::string prev_hash = last_block.hash;
    int new_index = last_block.index + 1;
    int difficulty = 2;

    Block block = Block(
        new_index,
        generate_hash_with_zeros(stoi(difficulty)),
        prev_hash,
        0,
        time(&timestamp),
        0,
        stoi(difficulty),
        {},
        false
    );
}