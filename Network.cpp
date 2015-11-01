#include "Network.h"
#include <iostream>
#include "Display.h"
#include <sstream>
ENetHost* server=NULL;

using namespace std;

void handleServerPacket(ENetPacket* pack, ENetPeer* peer){





}

void handleClientPacket(ENetPacket* pack, ENetPeer* peer){
    cout<<"Pack recieved"<<endl;
    char type=(pack->data)[0];
    switch(type){
        case(PACK_GUNGEON):{
            int w,h;
            stringstream ss;
            ss.write((char*)&(pack->data[1]),pack->dataLength-1);
            ss.read((char*)&w,4);
            ss.read((char*)&h,4);
            Zone* z=new Zone(w,h);
            for(int i=0;i<w*h;i++)
                z->tiles[i].read(ss);

            theDisplay->setZone(z);

        }break;
        case(PACK_MSG):{
            theDisplay->addMsg(string((char*)&pack->data[2],(uint8_t)pack->data[1]));

        }break;
        case(PACK_PLAYER):{
            int hp;
            float x,y;

            hp=*((int*)&pack->data[1]);
            x=*((float*)&pack->data[5]);
            y=*((float*)&pack->data[9]);

            theDisplay->updatePlayer(hp,x,y);


        }break;





    }


}

