#ifndef GLHELP_H_INCLUDED
#define GLHELP_H_INCLUDED
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameOptions.h"



void setupGL(){
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);

    //glViewport(0, 0, gameOptions.getInt("XRES",800), gameOptions.getInt("YRES",600));


    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);


    glEnable ( GL_TEXTURE_2D ) ;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setupShaders();

    }

void shutdownGL(){
    cleanupShaders();


}



#endif // GLHELP_H_INCLUDED
