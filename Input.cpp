#include "Input.h"
#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include "GameOptions.h"


int gameInput(bool &quit){
    SDL_Event event;

        while (SDL_PollEvent(&event)){

            // check for messages
            switch (event.type){
                // exit if the window is closed
            case SDL_QUIT:
                quit=true;
                break;

            case SDL_MOUSEMOTION:{


            break;}

            case SDL_MOUSEWHEEL:{

            break;}

            // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed

                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        quit=true;
                        }
                break;
                }

                case SDL_KEYUP:
                {


                    break;
                }
                /*case SDL_WINDOWEVENT:
                    if(event.window.event==SDL_WINDOWEVENT_FOCUS_LOST){
                        hasFocus=false;
                        mainCam.vel=glm::vec3();
                    }
                    if(event.window.event==SDL_WINDOWEVENT_FOCUS_GAINED){
                        hasFocus=true;
                    }
                break;*/

            } // end switch
        } // end of message processing
    return 0;
    }




