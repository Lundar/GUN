#include "Render.h"
#include "ShaderHelp.h"
#include "GameOptions.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PROJECTION_MATRIX glm::perspective(gameOptions.getFloat("FOV",67), (gameOptions.getInt("XRES",800))/((float)gameOptions.getInt("YRES",600)), 1.0f, 5000.0f)

void setupRender(){

    glUseProgram(getShader("shad"));
	glm::mat4 temp;
	temp=PROJECTION_MATRIX;
	int tempLoc=glGetUniformLocation(getShader("shad"), "projectionMatrix");
	glUniformMatrix4fv(tempLoc,1, GL_FALSE,&temp[0][0]);

}

void renderLoop(SDL_Window* screen){

    /*mainCam.setup();
    mainCam.pos=glm::vec3(-16,-8,-8);
    mainCam.angle=glm::vec3(2.14,3.141/2,0);
    mainCam.toggleMode();*/

    bool quit=false;


    while(!quit) {
        gameInput(quit);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*glm::mat4 viewMatrix = mainCam.getView();


        if(present<0){
            renderPlate(viewMatrix);

            print->render(viewMatrix);

            if(pSpot != glm::vec4())
            renderSelection(viewMatrix);

            renderArrow(viewMatrix);
            hud.render();
        }else{
            present+=0.01;
            print->render(viewMatrix,present);
        }*/


//printf("%s\n",gluErrorString(glGetError()));

        SDL_GL_SwapWindow(screen);
    }

}
