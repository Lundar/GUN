#include "CreatureBreeder.h"

CreatureBreeder theBreeder;

CreatureBreeder::CreatureBreeder():namer("names.txt"){
    uuidGen=1;
}

CreatureBreeder::~CreatureBreeder(){
    //dtor
}

void CreatureBreeder::breed(Creature &c){
    c.name=namer.genName();
    c.uuid=uuidGen++;
    c.scrnChar='@';
    c.hp=10;
    c.layer=0;

}
