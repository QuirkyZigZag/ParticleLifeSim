#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <iostream>
#include"Components.hpp"

struct Rectangle{

    float x,y, width, height;
    Rectangle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height){}


    bool ContainsParticle(const Entity& entity)
    {
        TransformComponent particle = entity.getComponent<TransformComponent>();
        Vector2 pos = entity.getComponent<TransformComponent>().Position;
        //box collision
        return ( pos.x>=x - width/2  && pos.x <= x+width/2
                && pos.y >= y - height/2 && pos.y <= y + height/2);
    }


    bool intersects(const Rectangle& range){

          return !(range.x - range.width /2 > x + width /2 ||
                  range.x + range.width /2 < x - width /2 ||
                  range.y -range.height /2 > y + height /2 ||
                  range.y + range.height /2 < y - height /2);
    }
       
};


class Quadtree
{

    private:

        static const int CAPACITY = 4;
        std::vector<Entity*> particles;
        bool isDivided = false;

        Rectangle boundary;

        Quadtree* northeast = nullptr;
        Quadtree* northwest = nullptr;
        Quadtree* southeast = nullptr;
        Quadtree* southwest = nullptr;

    public:

    Quadtree(Rectangle& boundary);
    //maybe make a reference depnds see later
    ~Quadtree();

    bool insert(Entity& ent);
    void subdivide();
    std::vector<Entity*> query(Rectangle& range);
    void clear();
    void update(const std::vector<Entity*>& allParticles);




};




















#endif