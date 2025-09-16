#include "../include/Collision.hpp"

bool Collision::AABB(const SDL_Rect & recA, const SDL_Rect & recB){

    if(
        recA.x + recA.w*0.5 >= recB.x &&
        recB.x + recB.w*0.5 >= recA.x &&
        recA.y + recA.h*0.5 >= recB.y &&
        recB.y + recB.h*0.5 >= recA.y
    )
    {
        return true;
    }

    return false;
}


bool Collision::AABB(const BoxCollider& colA, const BoxCollider& colB){
        
        if(AABB(colA.collider,colB.collider))
        {
            //std::cout << colA.tag << " hit " << colB.tag << std::endl;
            return true;
            
        }
        return false;



}

void Collision::ResolveCollision(BoxCollider& colA, BoxCollider& colB){
    if (!AABB(colA, colB)) return;

    TransformComponent* tA = colA.transform;
    TransformComponent* tB = colB.transform;

    Vector2 delta = tB->Position - tA->Position;
    float dist = Vector2::Magnitude(delta);
    if (dist == 0) dist = 0.1f;
    Vector2 resolution = (delta / dist) * 0.1f;
    tA->Position -= resolution;
    tB->Position += resolution;
    //std::swap(tA->velocity, tB->velocity);
}

