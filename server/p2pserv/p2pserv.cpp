#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "entities.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread"
#include "iostream"
#include <string>
#include "write_and_read.h"
#include <mutex>
#include "json.hpp"
using json = nlohmann::json;
std::mutex mtx;
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void handle_client(SOCKET ClientSocket, 
    std::string clientIP, 
    Node& node, 
    std::vector<Block>& blockchain,
    Mempool& mempool,
    std::vector<User>& users
    ) {

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int iResult;
    int iSendResult;
    

    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            std::string message(recvbuf, iResult);
            if(message.find("NEW_TRANSACTION") != std::string::npos){
                std::string json_str = message.substr(16);
                json j = json::parse(json_str);

                Transaction tx;
                tx.txid            = j["txid"];
                tx.timestamp       = j["timestamp"];
                tx.value_btc       = j["value_btc"];
                tx.fee             = j["fee"];
                tx.vsize           = j["vsize"];
                tx.fee_per_vbyte   = j["fee_per_vbyte"];
                tx.nb_input        = j["nb_input"];
                tx.nb_output       = j["nb_output"];
                tx.nb_adresses_src = j["nb_adresses_src"];

                for(auto& a : j["adresses_sources"])
                    tx.adresses_sources.push_back(a);

                for(auto& a : j["adresses_dest"])
                    tx.adresses_dest.push_back(a);

                for(auto& u : j["input_utxos"])
                    tx.input_utxos.push_back(UTXO(u["amount"], u["public_key"], u["txid_transaction"], u["vout"]));

                for(auto& u : j["output_utxos"])
                    tx.output_utxos.push_back(UTXO(u["amount"], u["public_key"], u["txid_transaction"], u["vout"]));

                std::lock_guard<std::mutex> lock(mtx);
                mempool.transactions.push_back(tx);
                export_mempool_in_registre("./registre/mempool.json", mempool);
            } else if(message.find("NEW_BLOCK") != std::string::npos){
                std::string json_str = message.substr(10);
                json j = json::parse(json_str);

                Block block;
                block.index      = j["index"];
                block.hash       = j["hash"];
                block.prevHash   = j["prevHash"];
                block.timestamp  = j["timestamp"];
                block.nonce      = j["nonce"];
                block.difficulty = j["difficulty"];
                block.resolved   = j["resolved"];

                for(auto& t : j["transactions"]){
                    Transaction transaction;
                    transaction.txid            = t["txid"];
                    transaction.timestamp       = t["timestamp"];
                    transaction.value_btc       = t["value_btc"];
                    transaction.fee             = t["fee"];
                    transaction.vsize           = t["vsize"];
                    transaction.fee_per_vbyte   = t["fee_per_vbyte"];
                    transaction.nb_input        = t["nb_input"];
                    transaction.nb_output       = t["nb_output"];
                    transaction.nb_adresses_src = t["nb_adresses_src"];

                    for(auto& a : t["adresses_sources"])
                        transaction.adresses_sources.push_back(a);

                    for(auto& a : t["adresses_dest"])
                        transaction.adresses_dest.push_back(a);

                    for(auto& u : t["input_utxos"])
                        transaction.input_utxos.push_back(UTXO(u["amount"], u["public_key"], u["txid_transaction"], u["vout"]));

                    for(auto& u : t["output_utxos"])
                        transaction.output_utxos.push_back(UTXO(u["amount"], u["public_key"], u["txid_transaction"], u["vout"]));
                
                    
                    block.transactions.push_back(transaction);
            }

                std::lock_guard<std::mutex> lock(mtx);
                blockchain.push_back(block);
                export_blockchain_in_registre("./registre/blockchain.json", blockchain);
            } else if(message.find("NEW_USER") != std::string::npos) {
                std::string json_str = message.substr(9);
                json j = json::parse(json_str);

                std::vector<UTXO> utxos;
                for(auto& u : j["utxos"]){
                    UTXO utxo(
                        u["amount"],
                        u["public_key"],
                        u["txid_transaction"],
                        u["vout"]
                    );
                    utxos.push_back(utxo);
                }
                User user(
                    j["privateKey"],
                    j["publicKey"],
                    j["address"],
                    j["balance"],
                    utxos
                );

                std::lock_guard<std::mutex> lock(mtx);
                users.push_back(user);
                export_user_in_registre("./registre/users.json", users);
            } else if(message == "PING"){
                iSendResult = send(ClientSocket, "PONG", 4, 0); 
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                }
            }
        }
    } while(iResult > 0);

    for(Peer& peer : node.peers) {
        if(peer.ip == clientIP){
            peer.connected = false;
        }
    }
}

int __cdecl start_server(
    Node& node, 
    std::vector<User>& users,
    std::vector<Block>& blockchain, 
    Mempool& mempool) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;
    struct sockaddr_in clientAddr;

    int clientAddrLen = sizeof(clientAddr);
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    while(true){
        // Accept a client socket
        ClientSocket = accept(ListenSocket, (sockaddr*)&clientAddr, &clientAddrLen);

        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            continue;
        }
        
        std::string clientIP = inet_ntoa(clientAddr.sin_addr);
        Peer newPeer(clientIP, std::stoi(DEFAULT_PORT), true);
        node.peers.push_back(newPeer);

        std::thread(handle_client, 
            ClientSocket, 
            clientIP, 
            std::ref(node),
            std::ref(blockchain),
            std::ref(mempool),
            std::ref(users)
        ).detach();
    }

    // shutdown the connection since we're done
    shutdown(ClientSocket, SD_SEND);
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}