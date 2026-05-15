#ifndef WRITE_AND_READ_PEERS_H
#define WRITE_AND_READ_PEERS_H

#include <string>
#include <vector>

#include "entities.h"

void write_peers(std::vector<Node>& nodes);
std::vector<Peer> read_peers();

#endif
