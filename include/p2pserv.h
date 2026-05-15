#ifndef P2PSERV_H
#define P2PSERV_H

#include <windows.h>
#include <string>
#include "entities.h"

void handle_client(SOCKET ClientSocket, 
    std::string clientIP, 
    Node& node, 
    std::vector<Block>& blockchain,
    Mempool& mempool,
    std::vector<User>& users
);
int __cdecl start_server(
    Node& node, 
    std::vector<User>& users,
    std::vector<Block>& blockchain, 
    Mempool& mempool);

#endif
