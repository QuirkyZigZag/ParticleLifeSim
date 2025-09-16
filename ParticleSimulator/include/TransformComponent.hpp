#pragma once

#ifndef PCOMP_H
#define PCOMP_H
#include"Entity.hpp"
#include "Components.hpp"
#include "Vector2.hpp"


class TransformComponent: public Component
{
    

    public:

     Vector2 Position;
     Vector2 velocity;

     int height = 32;
     int width = 32;
     float scale = 1;

     int speed = 3;



    TransformComponent(){
        Position = Vector2(0,0);
    }

    TransformComponent(float sc){
        Position = Vector2(0,0);
        scale = sc;
    }
    TransformComponent(float x, float y){
        Position = Vector2(x,y);
    }

    TransformComponent(int x, int y){
        Position = Vector2(x,y);
    }

    TransformComponent(float x, float y,float sc){
        Position = Vector2(x,y);
        scale = sc;
    }

    TransformComponent(int x, int y,float sc){
        Position = Vector2(x,y);
        scale = sc;
    }
    TransformComponent(float x, float y,int h, int w, int scl){
        Position = Vector2(x,y);
        height = h;
        width = w;
        scale = scl;
    }
    TransformComponent(int x, int y,int h, int w, float scl){
        Position = Vector2(x,y);
        height = h;
        width = w;
        scale = scl;
    }

    void init() override
    {
        velocity = Vector2(0,0);
    }



    void update() override
    {
    
    
    }


};
#endif