#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H
#include <string>

#include "Entity.hpp"

#include "SDL2/SDL.h"
#include "Components.hpp"


class PhysicsComponent : public Component
{

    private:
    
    float mass = 1.0f;
    
    public:

    Vector2 velocity;
    Vector2 acceleration;
    float dt = 0.0f;


    PhysicsComponent(){

        velocity = Vector2(0,0);
        acceleration = Vector2(0,0);

    }

    PhysicsComponent(Vector2 vel){
        velocity = vel;
        acceleration = Vector2(0,0);
    }

    PhysicsComponent(Vector2 vel, Vector2 accel){
        velocity = vel;
        acceleration = accel;
    }

    void update() override
    {
        auto &transform = entity->getComponent<TransformComponent>();
        velocity += acceleration;
        transform.Position += velocity;
        acceleration = Vector2(0,0);
        
    }
    void ApplyForce(Vector2 Force, float deltaTime ){
        (acceleration += Force/mass) * deltaTime;

    }

    void SetVelocity(Vector2 vel){
        velocity = vel;
        acceleration = Vector2(0,0);
    }

    void SetAcceleration(Vector2 acc) {acceleration = acc;}


};

#endif