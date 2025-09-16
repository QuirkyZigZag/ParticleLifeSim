

#include"../include/TextureManager.hpp"
#include"../include/game.hpp"
#include<iostream>
#include<string>
#include <ostream>

using namespace std;

SDL_Texture* TextureManager::Load_Texture(const char* texture)
{

    SDL_Surface* tempSurface = IMG_Load(texture);
    if(tempSurface == nullptr){
        std::cout << "no surface" << std::endl;
    }
    SDL_Texture * tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    //std::cout <<"texture:" << &tex << std::endl;

    return tex;

}





void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, tex, &src, & dest);

}