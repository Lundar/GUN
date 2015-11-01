#ifndef CREATURE_H
#define CREATURE_H
#include <iostream>
#include <inttypes.h>

using namespace std;

class Creature{
public:
    Creature();
    virtual ~Creature();

    void read(istream &in);
    void write(ostream &out);
    void setPos(float, float);

    string name;
    uint16_t scrnChar;
    int hp;
    int layer;
    float posX;
    float posY;

protected:

    int uuid;
    friend class CreatureBreeder;

};

#endif // CREATURE_H
