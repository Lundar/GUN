#ifndef NAMEGEN_H
#define NAMEGEN_H
#include <iostream>
#include <map>
#include <inttypes.h>

using namespace std;

///Assume that srand is called
class NameGen{
public:
    NameGen(string file);
    ~NameGen();

    string genName(int max = 20);
    void train(string file);

protected:
    map<uint16_t,map<char,uint32_t> > mark;


};

#endif // NAMEGEN_H
