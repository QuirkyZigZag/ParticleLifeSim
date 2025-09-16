#pragma once

#include "SDL2/SDL.h"
#include<iostream>
#include"BoxCollider.hpp"


class Collision{

    public:
    static bool AABB(const SDL_Rect& recA, const SDL_Rect & recB);
    static bool AABB(const BoxCollider& colA, const BoxCollider& colB);



    static void ResolveCollision(BoxCollider& colA, BoxCollider& colB);
        
        
    



};

