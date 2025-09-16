#pragma once
#include<iostream>
#include "Components.hpp"
#include "SDL2/SDL.h"
#include<string>

class SpriteComponent: public Component
{
    private:
        TransformComponent * transform;
        ParticleComponent * particle;
        float particleSize;;

        

        SDL_Texture * texture;
        SDL_Rect srcRect,destRect;


    public:

        

        SpriteComponent() = default;


        SpriteComponent(const char* path)
        {
            setTexture(path);

        }

        void setTexture(const char * path){
    
           // std::cout << path << std::endl;
            texture = TextureManager::Load_Texture(path);
            if (texture == nullptr) {
            std::cout << "SDL Error: " << SDL_GetError() << std::endl;
            }
            //std::cout << texture << std::endl;
            
        }



        ~SpriteComponent(){
            SDL_DestroyTexture(texture);
        }




        void init() override
        {
            

            transform = &entity->getComponent<TransformComponent>();
            if(transform==nullptr){
                std::cout << "Tried to add Sprite Component but no transform componenet found" << "\n";
                return;
            }


            particle  = &entity->getComponent<ParticleComponent>();

            if(particle == nullptr){
                particleSize = 1;
            }
            else{
                particleSize = particle->size;
            }

            float scale = transform->scale;
            

            destRect.x = static_cast<int>(transform->Position.x);
            destRect.y = static_cast<int>(transform->Position.y);
            srcRect.w = static_cast<int>(transform->width);
            srcRect.h = static_cast<int>(transform->height);
            srcRect.x = 0;
            srcRect.y = 0;

            destRect.w = static_cast<int>(transform->width * scale *particleSize);
            destRect.h = static_cast<int>(transform->height * scale*particleSize);

        }

        void update() override
        {
            destRect.x = static_cast<int>(transform->Position.x);
            destRect.y = static_cast<int>(transform->Position.y);

        }

        void draw() override
        {

            if (texture == nullptr){
                std::cout << "no texture" <<std::endl;
            }
            TextureManager::Draw(texture, srcRect,destRect);

        }



};

