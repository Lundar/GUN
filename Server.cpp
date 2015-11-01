#define SDL_MAIN_HANDLED

#include <iostream>
#include <signal.h>
#include "Network.h"
#include "World.h"
#include "GameOptions.h"
#include <time.h>

using namespace std;

bool serverRunning = true;
map<ENetPeer*,Creature*> players;

void stop(int u){
    if(serverRunning)
        serverRunning=false;
    else
        exit(2);
}

void pumpServer(){
    ENetEvent event;
    while (enet_host_service (server, & event, 50) > 0){
        switch (event.type){
        case ENET_EVENT_TYPE_CONNECT:
            event.peer -> data = NULL;
            if(event.data!=PROTOCOL_VERSION)
                enet_peer_disconnect(event.peer,2);
            cout<<event.peer->address.host<<":"<<event.peer->address.port<<" Connected"<<endl;
            enet_peer_send(event.peer,0,makeGungeonPacket(theWorld->getZone(0)));
            players[event.peer]=theWorld->addPlayer();
            enet_peer_send(event.peer,0,makeMessagePacket(players[event.peer]->name+", you have been sent to the gungeon for your crimes."));
            players[event.peer]->setPos(5,5);
            enet_peer_send(event.peer,0,makePlayerDataPacket(players[event.peer]));

        break;
        case ENET_EVENT_TYPE_RECEIVE:
            handleServerPacket(event.packet,event.peer);
            enet_packet_destroy (event.packet);
        break;

        case ENET_EVENT_TYPE_DISCONNECT:
            cout<<"someone disconnected"<<endl;
            event.peer -> data = NULL;
        }
    }

}

int main(){

    if (enet_initialize () != 0){
        cout<<"An error occurred while initializing ENet."<<endl;
        return 1;//EXIT_FAILURE;
    }
    atexit (enet_deinitialize);

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = gameOptions.getInt("PORT",13699);
    cout<<address.port<<endl;

    server = enet_host_create (&address, 2048, 2, 0, 0);

    if (server == NULL){
        cout<<"An error occurred while trying to create an ENet server host.\n";
       return 1;
    }

    srand(time(0));
    signal(SIGINT,stop);

    theWorld = new World();

    while(serverRunning){
        pumpServer();
        theWorld->tickWorld(1);//TODO change to 30 TPS model


    }

    delete theWorld;

    enet_host_destroy(server);
    return 0;
}
