#pragma once
#ifndef VECTOR2_H
#define VECTOR2_H
#include<cmath>
#include<iostream>

struct Vector2{

    float x, y;
    Vector2(){
        x = 0;
        y = 0;
    }

    Vector2(float xval, float yval){
        x = xval;
        y = yval;
    }

    Vector2 operator+(const Vector2 & other) {return Vector2(x+other.x, y+other.y);}
    Vector2 operator-(const Vector2 & other){return Vector2(x-other.x,y-other.y);}
    Vector2 operator*(float scalar){return Vector2(x*scalar, y*scalar);}
    Vector2 operator/(float scalar){return Vector2(x/scalar, y/scalar);}
    static Vector2 Zero(){return Vector2(0,0);}

    Vector2 operator+=(const Vector2 & other)
    {
        x += other.x;
        y+= other.y;
        return * this;
    }

    Vector2 operator-=(const Vector2 & other)
    {
        x -= other.x;
        y-= other.y;
        return * this;
    }

    Vector2 operator*=(float scalar){
        //std::cout << "Hello this is multiplying *=" << "\n";
        x*=scalar;
        y*=scalar;
        return *this;
    }

    static float Magnitude(Vector2 vec)
    {
        return sqrt((vec.x)*vec.x + (vec.y)*vec.y);
    }

    static Vector2 getDirection(Vector2 pos1, Vector2 pos2)
    {

        return Vector2(pos2.x-pos1.x, pos2.y-pos1.y);

    }
    static Vector2 getNormalised(Vector2 vec)
    {
        return vec/(Magnitude(vec));

    }

    static Vector2 getNormalisedDirectionVector(Vector2 vec1, Vector2 vec2)
    {
        return getNormalised(getDirection(vec1, vec2));
    }
    

    

};

#endif