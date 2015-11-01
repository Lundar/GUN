#include "Zone.h"
#include <tgmath.h>

Zone::Zone(int x, int y){
    width=x;
    height=y;
    tiles= new Tile[x*y];

    for(int i=0;i<200;i++)
        tiles[rand()%(x*y)].scrnChar='X';

}

Zone::~Zone(){
    if(tiles)
        delete[] tiles;
}

void Zone::tick(int dt){

}

Tile* Zone::getTile(int x, int y){
    return &(tiles[x+y*width]);
}

string Zone::getLine(int x, int y){
    string ret;

    if(y<height)
    for(int i=0;x+i<width;i++)
        ret+=(char)tiles[x+i+y*width].scrnChar;

    for(Creature c : crtrs){
    if(c.posX>x&&round(c.posY)==y)
        ret[round(c.posX)-x]=c.scrnChar;

    }

    return ret;
}
