#ifndef TILE_H
#define TILE_H
#include <inttypes.h>
#include <iostream>

class Tile{
public:
    Tile();
    ~Tile();

    void write(std::ostream &o);
    void read(std::istream &in);

    uint16_t scrnChar;
    bool passable;
    bool transparent;
    bool memorable;







};

#endif // TILE_H
