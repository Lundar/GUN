#include "Console.h"

#include "Textures.h"
#include "ShaderHelp.h"
#include "GLHelp.h"

bool init=false;

Console::Console(int w, int h, string name){
    cellSize=8;
    width=w;
    height=h;
    inputCall=NULL;

    inBuff= new uint16_t[w*h];
    outBuff= new uint16_t[w*h];
    memset(inBuff,0,w*h*2);


    if(init){
        cerr<<"Only one Console window is allowed at this time."<<endl;
        exit(1);
    }

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		exit(1);
	}
	atexit(SDL_Quit);

    screen = SDL_CreateWindow(name.c_str() ,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w*cellSize, h*cellSize*2, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    glcontext=SDL_GL_CreateContext(screen);
	SDL_ShowCursor(true);
	GLenum err = glewInit();
	if (GLEW_OK != err){
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

    if ( !screen ){
		printf("Unable to start video: %s\n", SDL_GetError());
		cout<<w<<"x"<<h<<endl;
		exit(1);
	}

    loadTextures();
	setupGL();
    init=true;
}

Console::~Console(){
    if(inBuff)
        delete[] inBuff;
    if(outBuff)
        delete[] outBuff;

    cleanupShaders();
	unloadTextures();
    SDL_GL_DeleteContext(glcontext);
    init=false;
}

void Console::resize(int px,int py){
    int nx, ny;
    nx=px/cellSize;
    ny=py/(cellSize*2);
    if(nx*cellSize!=px||ny*cellSize*2!=py)
        SDL_SetWindowSize(screen,nx*cellSize,ny*cellSize*2);

    if(inBuff)
        delete[] inBuff;
    if(outBuff)
        delete[] outBuff;

    inBuff= new uint16_t[nx*ny];
    outBuff= new uint16_t[nx*ny];
    memset(inBuff,0,nx*ny*2);

    width=nx;
    height=ny;
}

void Console::getSize(int* x, int* y){
    *x=width;
    *y=height;
}

void Console::swapRender(){
    glViewport(0, 0, width*cellSize, height*cellSize*2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(getShader("ui"));

    glm::mat4 proj = glm::ortho(0.0f,(float)width*cellSize,00.0f,(float)height*cellSize*2,-50.0f,50.0f);
    GLuint tempLoc=glGetUniformLocation(getShader("ui"), "projMatrix");
    glUniformMatrix4fv(tempLoc,1, GL_FALSE,&proj[0][0]);


    glm::mat4 size;
    tempLoc=glGetUniformLocation(getShader("ui"), "mode");
    glUniform1i(tempLoc,2);
    tempLoc=glGetUniformLocation(getShader("ui"), "textColor");
    glUniform3f(tempLoc,1,1,1);
    TextRenderer rend("TextAB");
    rend.setRenderSize(cellSize,cellSize*2);

    string s;
    memcpy(outBuff,inBuff,sizeof(uint16_t)*width*height);

    for(int x=0;x<height;x++){
        s="";
        for(int i=0;i<width;i++)
            s+=(char)outBuff[x*width+i];

        size= glm::translate(glm::mat4(),glm::vec3(0,(height-x)*cellSize*2,0));
        tempLoc=glGetUniformLocation(getShader("ui"), "modelMatrix");
        glUniformMatrix4fv(tempLoc,1, GL_FALSE,&size[0][0]);

        rend.renderString(s);
    }

    SDL_GL_SwapWindow(screen);
}

bool Console::doInput(){
    SDL_Event event;

    while (SDL_PollEvent(&event)){
        if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_c&&(event.key.keysym.mod&KMOD_CTRL))
            return true;
        if(event.type==SDL_QUIT)
            return true;
        if(event.type==SDL_WINDOWEVENT&&event.window.event==SDL_WINDOWEVENT_RESIZED){
            resize(event.window.data1,event.window.data2);
        }
        if(inputCall)
            inputCall(event);

    }
    return false;
}

void Console::setCallback(eventCallback in){
    inputCall=in;
}

void Console::drawChar(int x, int y, char c){
    inBuff[x+y*width]=c;
}

void Console::drawString(int x, int y, string s){

    for(int i=0;i<s.length()&&x+i<width;i++)
        inBuff[x+i+y*width]=s[i];

}
