#ifndef DISPLAY_H
#define DISPLAY_H
#include "Console.h"
#include "Zone.h"
#include <string>
#include <list>
#include <enet/enet.h>

using namespace std;

class Display{
public:
    Display();
    ~Display();

    void addMsg(string s);
    void update();
    void run();
    void pumpNet();
    void connect(string addr, int port);
    void setZone(Zone* z);
    void updatePlayer(int hp,float x, float y);

protected:
    int offX,offY;
    Console con;
    Zone* curZone;
    Creature* player;
    list<string> messages;

    ENetHost* host;
    ENetPeer* client;

    friend bool doInput(SDL_Event &e);

};

extern Display* theDisplay;

#endif // DISPLAY_H
