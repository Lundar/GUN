#include "World.h"
#include "CreatureBreeder.h"

World* theWorld=NULL;

World::World(){
    zones.push_back(new Zone(200,50));
}

World::~World(){
    for(int x=0;x<zones.size();x++)
        delete zones[x];
}

Zone* World::getZone(int i){
    if(i>=0&&i<zones.size())
        return zones[i];
    return NULL;
}

void World::tickWorld(int dt){
    for(int x=0;x<zones.size();x++)
        zones[x]->tick(dt);

}

Creature* World::addPlayer(){
    Creature c;
    theBreeder.breed(c);
    return zones[0]->addCrtr(c);
}
