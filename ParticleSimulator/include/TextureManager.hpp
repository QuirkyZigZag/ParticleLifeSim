#pragma once

#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include<iostream>
#include<SDL2/SDL.h>   
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include"game.hpp"


class TextureManager{
    public:

    static SDL_Texture * Load_Texture(const char* filename);
    static void Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest);
    static SDL_Texture* CreateSquareTexture(int size, SDL_Color color);



};



#endif