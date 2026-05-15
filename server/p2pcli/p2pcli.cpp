#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstdint>

#include "entities.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "27015"

int __cdecl start_client(Node& node, std::string message)
{
    if(node.peers.empty()){
        printf("Aucun peer connu\n");
        return 1;
    }

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    for(Peer& peer : node.peers){
        struct addrinfo *result = NULL, *ptr = NULL;
        SOCKET ConnectSocket = INVALID_SOCKET;

        int iResult = getaddrinfo(peer.ip.c_str(), DEFAULT_PORT, &hints, &result);
        if(iResult != 0){
            printf("getaddrinfo failed for %s: %d\n", peer.ip.c_str(), iResult);
            continue;
        }

        for(ptr = result; ptr != NULL; ptr = ptr->ai_next){
            ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if(ConnectSocket == INVALID_SOCKET){
                printf("socket failed: %ld\n", WSAGetLastError());
                continue;
            }

            iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if(iResult == SOCKET_ERROR){
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if(ConnectSocket == INVALID_SOCKET){
            printf("Unable to connect to %s\n", peer.ip.c_str());
            continue;
        }

        // Envoie la taille du message sur 4 bytes
        uint32_t msg_size = htonl((uint32_t)message.size());
        send(ConnectSocket, (char*)&msg_size, 4, 0);

        // Envoie le message
        iResult = send(ConnectSocket, message.c_str(), (int)message.size(), 0);
        if(iResult == SOCKET_ERROR){
            printf("send failed: %d\n", WSAGetLastError());
        } else {
            printf("Message envoye a %s (%d bytes)\n", peer.ip.c_str(), iResult);
        }

        shutdown(ConnectSocket, SD_SEND);
        closesocket(ConnectSocket);
    }

    return 0;
}