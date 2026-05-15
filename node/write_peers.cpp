#include <fstream>
#include <string>
#include "entities.h"
#include "iostream"
#include "write_and_read.h"

std::string path = "./registre/peers.json";

void write_peers(std::vector<Node>& nodes) {
    json arr = json::array();

    for(const Node& node : nodes){
        json j;
        j["ip"]   = node.ip;
        j["port"] = node.port;

        json peers = json::array();
        for(const Peer& peer : node.peers){
            json u;
            u["ip"] = peer.ip;
            u["port"] = peer.port;
            u["connected"] = peer.connected;
            peers.push_back(u);
        }
        j["peers"] = peers;
        arr.push_back(j);
    }

    std::ofstream file(path);
    file << arr.dump(4);
    file.close();
}

std::vector<Node> read_peers() {
    std::vector<Node> nodes; 

    std::ifstream file(path);
    if(!file.is_open()) return nodes;
    
    // Vérifie si vide
    file.seekg(0, std::ios::end);
    if(file.tellg() == 0) return nodes;
    file.seekg(0, std::ios::beg);  // remet le curseur au début
    
    json j;
    file >> j;
    file.close();

    for(auto& item : j){
        std::vector<Peer> peers;
        for(auto& u : item["peers"]){
            Peer peer(
                u["ip"],
                u["port"],
                u["connected"]
            );
            peers.push_back(peer);
        }
        Node node (
            item["ip"],
            item["port"],
            peers
        );
        nodes.push_back(node);
    }

    return nodes;
}