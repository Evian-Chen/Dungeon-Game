#pragma once
#include "Position.h"

class Entity
{
protected:
	Position pos;
	char icon;
	int vitality, focus = 0, speed, hitRate;
	int pAttack, mAttack, pDefense, mDefense;
	bool isDead;
	bool inBattle;
public:
	Entity() {};
	Entity(int startX, int startY) : pos(startX, startY) {};
	
	// update position
	// virtual void update();
};