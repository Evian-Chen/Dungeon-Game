/***********************************************************************
? File: mapElement.h
? Description: This header file declares global variables representing
				various elements that can exist within the game world.
***********************************************************************/

#pragma once
#include "Enemy.h"
#include "HotSpring.h"
#include "Role.h"
#include "RandomEvent.h"

// Game element vectors

// enemy vector
extern vector<Enemy*> enemies;

// shop vector
extern vector<Shop*> shops;

// random event
extern vector<RandomEvent*> randoms;

// hot spring vector
extern vector<HotSpring*> hotSprings;