#pragma once

#ifndef GAME_H
#define GAME_H
#include <vector>


#include<iostream>
#include<SDL2/SDL.h>


class Game{


    public:

    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool full_screen);
    
    void handleEvents();
    void update();
    void render();
    void cleanup();
    void spawnParticles(int numParticles);



    static SDL_Renderer * renderer;
    static SDL_Event event;

    bool running(){return isRunning;}

    private:
    int count;
    bool isRunning;
    SDL_Window * window;
    


};





#endif