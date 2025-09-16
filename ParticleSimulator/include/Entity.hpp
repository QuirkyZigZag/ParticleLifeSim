#pragma once

#ifndef ENTITY
#define ENTITY

#include "game.hpp"
#include "GameObject.hpp"
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <vector>
#include <iostream>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;

inline ComponentID getComponentID(){
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept{
    static ComponentID typeID = getComponentID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component{
public:
    Entity* entity;

    virtual void init(){}
    virtual void update(){}
    virtual void draw(){}

    virtual ~Component(){}
};


class Entity{
private:
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitSet componentBitset;

public:
    Entity(Manager& manager) : manager(manager){}

    // Update all components
    void update(){
        for (auto& c : components){
            c->update();
        }
    }

    // Draw all components
    void draw(){
        for (auto& c : components){
            c->draw();
        }
    }

    // Check if the entity is active
    bool isActive() const{return active;}

    void destroy(){active = false;}

    template <typename T>
    bool hasComponent() const{
        return componentBitset[getComponentTypeID<T>()];
    }
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        T* c = new T(std::forward<TArgs>(mArgs)...);
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitset[getComponentTypeID<T>()] = true;

        c->init(); //Initialize the component
        return *c;
    }
    template <typename T>
    T& getComponent() const{
        auto ptr = componentArray[getComponentTypeID<T>()];
        return *static_cast<T*>(ptr);
    }
};

// Manager class for managing entities
class Manager{
private:
    std::vector<std::unique_ptr<Entity>> entities; // List of entities

public:
    // Update all entities
    void update(){
        for (auto& e : entities){
            e->update();}
    }

    void draw(){
    for (auto& e : entities){
        if (e->isActive())
        {
            e->draw();
        }
    }
}
    void refresh(){
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),[](const std::unique_ptr<Entity>& mEntity){return !mEntity->isActive();}),std::end(entities));
    }
    Entity& addEntity(){
        Entity* e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }

    void clearEntities(){
        for(auto &e : entities){
            e->destroy();
        }
        refresh();

        entities.clear();
    }
};

#endif