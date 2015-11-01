#ifndef ZONE_H
#define ZONE_H
#include "Tile.h"
#include "Creature.h"
#include <string>
#include <list>
#include <enet/enet.h>

using namespace std;

class Zone{
public:
    Zone(int x, int y);
    ~Zone();

    Tile* getTile(int x, int y);
    string getLine(int x, int y);
    void tick(int dt);
    Creature* addCrtr(Creature c){crtrs.push_back(c); return &crtrs.back();}
    void removeCreature(int uid);


    list<Creature> crtrs;

protected:

    int width;
    int height;
    Tile* tiles;

    friend ENetPacket* makeGungeonPacket(Zone*);
    friend void handleClientPacket(ENetPacket* pack, ENetPeer* peer);

};

#endif // ZONE_H
