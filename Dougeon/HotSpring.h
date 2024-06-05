/***********************************************************************
 * File: HotSpring.h
 * Description: This header file defines the HotSpring class,
                which represents a hot spring location within the game world.
                Players can interact with hot springs to regain health or
                other benefits.
***********************************************************************/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Position.h"
#include "allConstants.h"

using namespace std;

/**
 * Represents a hot spring location within the game world.
 * Players can interact with hot springs to regain health or other benefits.
 */
class HotSpring
{
protected:
    /** Position of the hot spring on the map */
    Position pos;
    /** Character icon representing the hot spring on the map */
    char icon = HOTSPRING;
public:
    // Default constructor (creates a hot spring at an unspecified position)
    HotSpring() {};

    // Constructor that takes a Position object to set the hot spring's location
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