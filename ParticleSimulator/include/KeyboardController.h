
#pragma once
#include"game.hpp"
#include"Entity.hpp"
#include"Components.hpp"

class KeyboardController : public Component
{

    public:

        TransformComponent * transform;

        void init() override
        {
            transform =  &entity->getComponent<TransformComponent>();


        }

        void update() override
        {
            SDL_Event action = Game::event;
            auto type  = action.type;

            if(type == SDL_KEYDOWN)
            {
                switch(action.key.keysym.sym)
                {
                    case SDLK_w:
                    transform->velocity.y = -1;

                    break;

                    case SDLK_s:
                    transform->velocity.y = 1;

                    break;

                    case SDLK_d:
                    transform->velocity.x = 1;

                    break;

                    case SDLK_a:
                    transform->velocity.x = -1;

                    break;
                }
            }
            if(type == SDL_KEYUP)
            {

                switch(action.key.keysym.sym)
                {
                    case SDLK_w:
                    transform->velocity.y = 0;

                    break;

                    case SDLK_s:
                    transform->velocity.y = 0;

                    break;

                    case SDLK_d:
                    transform->velocity.x = 0;

                    break;

                    case SDLK_a:
                    transform->velocity.x = 0;

                    break;
                }

            }
        }

};