#pragma once
#include <string>

#include "Entity.hpp"

#include "SDL2/SDL.h"
#include "Components.hpp"


class BoxCollider : public Component
{

    public:

    SDL_Rect collider;
    std::string tag;


    TransformComponent * transform;

    BoxCollider(std::string t){
        tag = t;
    }

    void init() override
    {
        if(!entity->hasComponent<TransformComponent>()){
            entity->addComponent<TransformComponent>();
        }

        transform = &entity->getComponent<TransformComponent>();



    }


    void update() override
    {
        collider.x = static_cast<int>(transform->Position.x);
        collider.y = static_cast<int>(transform->Position.y);
        collider.w = static_cast<int>(transform->width * transform->scale);
        collider.h = static_cast<int>(transform->height * transform->scale);

        
    }


};

