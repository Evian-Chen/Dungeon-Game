#pragma once
#include "Position.h"

class Entity
{
protected:
	Position position;
	char eicon;
	int vitality, focus = 0, speed, hitRate;
	int pAttack, mAttack, pDefense, mDefense;
	bool isDead;
	bool inBattle;
public:
	Entity() {};
	Entity(Position pos) :position(pos.x, pos.y) {};
	Entity(int startX, int startY) : position(startX, startY) {};

	// update position
	// virtual void update();
};