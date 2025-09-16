#pragma once

#ifndef GAMEOBJ_H
#define GAMEOBJ_H
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include"game.hpp"
#include "TextureManager.hpp"

class GameObject{


    public:

        GameObject(const char* texturesheet, int x, int y);
        ~GameObject();


        void Update();
        void Render();


    private:

        int xpos;
        int ypos;
        SDL_Texture * objTexture;
        SDL_Rect srcRect, destRect;


};



#endif