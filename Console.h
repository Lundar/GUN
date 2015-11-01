#ifndef CONSOLE_H
#define CONSOLE_H
#include <SDL2/SDL.h>
#include <string>

using namespace std;

typedef bool (*eventCallback)(SDL_Event &e);

class Console{
public:
    Console(int w, int h, string name);
    ~Console();
    void swapRender();
    bool doInput();
    void resize(int px,int py);
    void getSize(int* x, int* y);

    void drawChar(int x, int y, char c);
    void drawString(int x, int y, string s);
    void setCallback(eventCallback in);



protected:
    int width, height;
    int cellSize;
    SDL_GLContext glcontext;
    SDL_Window* screen;
    uint16_t* inBuff;
    uint16_t* outBuff;
    eventCallback inputCall;

};

#endif // CONSOLE_H
