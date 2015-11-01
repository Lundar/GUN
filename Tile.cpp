#include "Tile.h"

using namespace std;

Tile::Tile(){
    scrnChar=32;
    passable=true;
    transparent=true;
    memorable=true;
}

Tile::~Tile(){
    //dtor
}

void Tile::write(ostream &o){
    o.put(scrnChar);
    o.put(transparent);
    o.put(memorable);
}

void Tile::read(istream &in){
    scrnChar=in.get();
    transparent=in.get();
    memorable=in.get();

}
