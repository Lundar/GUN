#ifndef CREATUREBREEDER_H
#define CREATUREBREEDER_H
#include "NameGen.h"
#include "Creature.h"

class CreatureBreeder{
public:
    CreatureBreeder();
    virtual ~CreatureBreeder();

    void breed(Creature &c);

protected:
    NameGen namer;
    int uuidGen;


};

extern CreatureBreeder theBreeder;

#endif // CREATUREBREEDER_H
