#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Shop
{
public:
    
    // void buyItem(Role& role, string item);

    // vector that store all product sold in the shop
    // item name: price
    // vector<string> itemsName = { "woodenSword", "hammer", "giantHammer", "woodenShield", "plateArmor", "robe", "holyGrail","shoes", "godsbeard", "teleportScroll" };
    
    
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

    
        
    
    // void listAllItem();
};
