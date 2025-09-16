#include"../include/GameObject.hpp"

#include<SDL2/SDL.h>
#include"../include/game.hpp"


GameObject::GameObject(const char * texturesheet, int x, int y)

{
    xpos = x;
    ypos = y;

    objTexture = TextureManager::Load_Texture(texturesheet);



}

void GameObject::Update()
{
    xpos ++;
    ypos =0;
    srcRect.h = 32;
    srcRect.w = 32;
    srcRect.x = 0;
    srcRect.y=0;
    destRect.x = xpos;
    destRect.y=ypos;
    destRect.w = destRect.h = srcRect.h*2;


}

void GameObject::Render(){

    SDL_RenderCopy(Game::renderer, objTexture, &srcRect,&destRect);
}