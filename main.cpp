#include <cstdlib>
#include "Display.h"
#include <iostream>
#include <time.h>
#include "GameOptions.h"

using namespace std;

int main ( int argc, char** argv ){

    if (enet_initialize () != 0){
        cout<<"An error occurred while initializing ENet."<<endl;
        return 1;//EXIT_FAILURE;
    }
    atexit (enet_deinitialize);
    srand(time(0));

    theDisplay=new Display();
    theDisplay->connect(gameOptions.getString("SERVER","127.0.0.1"),gameOptions.getInt("PORT",13699));
    theDisplay->run();

    delete theDisplay;

    cout<<"Game concluded"<<endl;

	//exit(0);
	return 0;
}
