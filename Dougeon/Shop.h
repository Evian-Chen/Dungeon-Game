#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Position.h"
#include "GameObj.h"
#include "allConstants.h"

using namespace std;

class Shop
{
protected:
    Position pos;
    char icon = SHOP;
public:
    Shop() {};
    Shop(Position p) : pos(p) {};

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

    vector<pair<string, int>> itemsName= { { "woodenSword", 10 },
                               { "hammer", 15 },
                               { "giantHammer", 20 },
                               { "woodenShield", 12 },
                               { "plateArmor", 25 },
                               { "robe", 8 },
                               { "holyGrail", 50 },
                               { "shoes", 5 },
                               { "godsbeard", 30 },
                               { "teleportScroll", 1 } };

    map<string, int> priceTag = { { "woodenSword", 10 },
                               { "hammer", 15 },
                               { "giantHammer", 20 },
                               { "woodenShield", 12 },
                               { "plateArmor", 25 },
                               { "robe", 8 },
                               { "holyGrail", 50 },
                               { "shoes", 5 },
                               { "godsbeard", 30 },
                               { "teleportScroll", 1 } };

    // gameObj method
    void render() {};
};
