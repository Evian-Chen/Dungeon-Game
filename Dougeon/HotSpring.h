#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Position.h"
#include "allConstants.h"

using namespace std;

class HotSpring
{
protected:
    Position pos;
    char icon = HOTSPRING;
public:
    HotSpring() {};
    HotSpring(Position p) : pos(p) {};

    // setter
    void setPos(Position p)
    {
        this->pos = p;
    }
    void setX(int x) { this->pos.x = x; }
    void setY(int y) { this->pos.y = y; }

    // getter
    int getX() { return this->pos.x; }
    int getY() { return this->pos.y; }
    char getIcon() { return this->icon; }
};