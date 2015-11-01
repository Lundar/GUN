#ifndef WORLD_H
#define WORLD_H
#include "Zone.h"
#include <vector>

class World{
public:
    World();
    ~World();
    void tickWorld(int dt);
    Zone* getZone(int i);
    Creature* addPlayer();
protected:
    vector<Zone*> zones;
};


extern World* theWorld;
#endif // WORLD_H
