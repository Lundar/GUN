#ifndef PACKET_H_INCLUDED
#define PACKET_H_INCLUDED
#include <enet/enet.h>
#include <string>

#define PACK_GUNGEON 1
#define PACK_MSG 2
#define PACK_PLAYER 3
#define PACK_CRTR 4
#define PACK_CMD 5

class Zone;
class Creature;

ENetPacket* makeCreatureDataPacket(Creature* c);
ENetPacket* makePlayerDataPacket(Creature* c);
ENetPacket* makeCommandPacket();
ENetPacket* makeMessagePacket(std::string s);
ENetPacket* makeGungeonPacket(Zone*);



#endif // PACKET_H_INCLUDED
