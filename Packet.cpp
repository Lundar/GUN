#include "Packet.h"
#include <sstream>
#include "Zone.h"
#include "Creature.h"

using namespace std;

ENetPacket* makeGungeonPacket(Zone* z){

    stringstream ss;
    ss.put(PACK_GUNGEON);

    ss.write((char*)&z->width,4);
    ss.write((char*)&z->height,4);

    for(int x=0;x<z->width*z->width;x++)
        z->tiles[x].write(ss);

    return enet_packet_create (ss.str().c_str(), ss.str().length() , ENET_PACKET_FLAG_RELIABLE);
}

ENetPacket* makeMessagePacket(string s){
    string tmp=(char)PACK_MSG +((char)s.length() + s);

    return enet_packet_create (tmp.c_str(), tmp.length() , ENET_PACKET_FLAG_RELIABLE);
}

ENetPacket* makePlayerDataPacket(Creature* c){

    stringstream ss;

    ss.put(PACK_PLAYER);
    //ss.put(c->name.length());
    //ss.write(c->namec_str(),c->name.length());

    ss.write((char*)&c->hp,4);
    ss.write((char*)&c->posX,4);
    ss.write((char*)&c->posY,4);

    return enet_packet_create (ss.str().c_str(), ss.str().length() , ENET_PACKET_FLAG_RELIABLE);
}
