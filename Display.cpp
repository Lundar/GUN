#include "Display.h"
#include <time.h>
#include "Network.h"

Display* theDisplay=NULL;

bool doInput(SDL_Event &e){

    if(e.type==SDL_KEYDOWN){
        if(e.key.keysym.sym==SDLK_UP)
            theDisplay->player->posY-=1;
        if(e.key.keysym.sym==SDLK_DOWN)
            theDisplay->player->posY+=1;
        if(e.key.keysym.sym==SDLK_RIGHT)
            theDisplay->player->posX+=1;
        if(e.key.keysym.sym==SDLK_LEFT)
            theDisplay->player->posX-=1;
    }


}


Display::Display():con(80,24,"GUN"){
    offX=0;
    offY=0;
    curZone=NULL;
    con.setCallback(doInput);

    host = enet_host_create (NULL, 1 , 2 , 0 ,0 );
    client=NULL;

}

Display::~Display(){
    if(curZone)
        delete curZone;
}

void Display::run(){


    while(!con.doInput()){
        update();
        con.swapRender();
        pumpNet();

    }


}

void Display::addMsg(string s){
    messages.push_front(s);
}

void Display::setZone(Zone* z){
    if(curZone)
        delete curZone;
    curZone=z;
    player=curZone->addCrtr(Creature());
}

void Display::connect(string addr, int port){
    ENetAddress address;
    //ENetEvent event;
    enet_address_set_host (&address, addr.c_str());
    address.port = port;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    client = enet_host_connect (host, &address, 2, PROTOCOL_VERSION);

    if (client == NULL){
        fprintf (stderr, "No available peers for initiating an ENet connection.\n");
    }
}

void Display::updatePlayer(int hp,float x, float y){
    player->hp=hp;
    player->posX=x;
    player->posY=y;

    //TODO adjust screen center

}

void Display::pumpNet(){
    ENetEvent event;
    while (enet_host_service (host, & event, 50) > 0){
        switch (event.type){
        case ENET_EVENT_TYPE_CONNECT:
            event.peer -> data = NULL;
            cout<<"Connected Sucessfully"<<endl;

        break;
        case ENET_EVENT_TYPE_RECEIVE:
            handleClientPacket(event.packet,event.peer);
            enet_packet_destroy (event.packet);
        break;

        case ENET_EVENT_TYPE_DISCONNECT:
            cout<<"disconnected"<<endl;
            event.peer -> data = NULL;
            client=NULL;
        }
    }
}

void Display::update(){
    int w,h;
    con.getSize(&w,&h);
    if(curZone){
        for(int i=0;i<h*4/5;i++)
            con.drawString(0,i,curZone->getLine(offX,offY+i));
    }
    //draw status updates

    list<string>::iterator it=messages.begin();
    for(int i=h*4/5+1;i<h&&it!=messages.end();i++){
        con.drawString(0,i,*it);
        it++;
    }
    if(messages.size()>h*2/5)
        messages.pop_back();

}
