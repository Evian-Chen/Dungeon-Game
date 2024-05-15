#pragma once
#include <iostream>
#include <map>
using namespace std;

class Equipment
{
public:
	
	// when role buy these in shop, turn them to true

	map<string, int> items = { { "woodenSword", 10 },
							   { "hammer", 15 },
							   { "giantHammer", 20 },
							   { "woodenShield", 12 },
							   { "plateArmor", 25 },
							   { "robe", 8 },
							   { "holyGrail", 50 },
							   { "shoes", 5 },
							   { "godsbeard", 30 },
							   { "teleportScroll", 1 } };

	// weapons
	// once the role has the equipment, set their skill status to true
	bool hasWoodenSword;
	bool hasHammer;
	bool hasGiantHammer;
	
	// armor
	bool hasWoodenShield;
	bool hasPlateArmor;
	bool hasRobe;

	// accessory
	bool hasHolyGrail;
	bool hasShoes;

	// item
	bool hasGodsbeard;
	bool hasTeleportScroll;
};
