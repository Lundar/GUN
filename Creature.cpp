#include "Creature.h"

Creature::Creature(){
    scrnChar='@';
    uuid=-1;
}

Creature::~Creature(){
    //dtor
}

void Creature::setPos(float x, float y){
    posX=x;
    posY=y;

}
