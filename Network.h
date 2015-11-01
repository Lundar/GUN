#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include <enet/enet.h>
#include "Packet.h"

#define PROTOCOL_VERSION 2015

extern ENetHost* server;


void handleServerPacket(ENetPacket* pack, ENetPeer* peer);
void handleClientPacket(ENetPacket* pack, ENetPeer* peer);


#endif // NETWORK_H_INCLUDED
