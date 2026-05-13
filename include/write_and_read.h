#ifndef WRITE_AND_READ_H
#define WRITE_AND_READ_H

#include <string>
#include "entities.h"
#include "json.hpp"
using json = nlohmann::json;

void is_file_exist(std::string path);

std::vector<User> read_users(std::string path);
std::vector<Block> read_blockchain(std::string path);
Mempool read_mempool(std::string path);

void export_blockchain_in_registre(std::string path, std::vector<Block>& blocks);
void export_user_in_registre(std::string path, std::vector<User>& users);
void export_mempool_in_registre(std::string path, Mempool& mempool);


#endif
