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

std::vector<Peer> read_peers() {
    std::vector<Peer> peers;

    std::ifstream file(path);
    if(!file.is_open()) return peers;

    file.seekg(0, std::ios::end);
    if(file.tellg() == 0) return peers;
    file.seekg(0, std::ios::beg);

    json j;
    file >> j;
    file.close();

    for(auto& item : j){
        Peer peer(
            item["ip"],
            item["port"],
            false  // pas encore connecté au démarrage
        );
        peers.push_back(peer);
    }
    return peers;
}