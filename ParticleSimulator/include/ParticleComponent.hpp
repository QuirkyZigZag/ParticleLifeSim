#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include <string>

#include "Entity.hpp"

#include "SDL2/SDL.h"
#include "Components.hpp"


class ParticleComponent : public Component
{

    private:
    int id;
    
    public:
    float size;
    std::string color;
    TransformComponent * transform;

    ParticleComponent(){
        size = 1.0f;


    }
    ParticleComponent(float p_scale){
        size = p_scale;

    }

    void init() override
    {


    }

    void update() override
    {

        
    }

    //float getMass() {return mass;
    };

#endif