// Ensure this header is included only once in a single compilation
#pragma once

#include <iostream>     // For standard I/O operations
#include <string>       // For using std::string
#include <vector>       // For using std::vector
#include <map>          // For using std::map
#include "Position.h"   // Include custom Position class header
#include "allConstants.h" // Include all constants used in the program

using namespace std;    // Use the standard namespace

// Define the Shop class
class Shop
{
protected:
    Position pos;       // Position of the shop
    char icon = SHOP;   // Icon representing the shop, initialized to SHOP constant
public:
    // Default constructor
    Shop() {};

    // Constructor with position parameter
    Shop(Position p) : pos(p) {};

    // Setter for position
    void setPos(Position p)
    {
        this->pos = p; // Set the shop's position to the provided position
    }

    // Setter for the x-coordinate of the shop's position
    void setX(int x) { this->pos.x = x; }

    // Setter for the y-coordinate of the shop's position
    void setY(int y) { this->pos.y = y; }

    // Getter for the x-coordinate of the shop's position
    int getX() { return this->pos.x; }

    // Getter for the y-coordinate of the shop's position
    int getY() { return this->pos.y; }

    // Getter for the shop's icon
    char getIcon() { return this->icon; }

    Position getPos() { return this->pos; }

    // List of items available in the shop with their names and respective quantities
    vector<pair<string, int>> itemsName = {
        { "woodenSword", 10 },
        { "giantHammer", 20 },
        { "ritualSword", 30 },
        { "woodenShield", 12 },
        { "plateArmor", 20 },
        { "robe", 8 },
        { "holyGrail", 50 },
        { "teleportScroll", 1 }
    };

    // Map of items available in the shop with their names and respective prices
    map<string, int> priceTag = {
        { "woodenSword", 10 },
        { "giantHammer", 20 },
        { "ritualSword", 30 },
        { "woodenShield", 12 },
        { "plateArmor", 20 },
        { "robe", 8 },
        { "holyGrail", 50 },
        { "teleportScroll", 1 }
    };
};
