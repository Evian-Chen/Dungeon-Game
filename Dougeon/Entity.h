#pragma once
#include "Position.h"
#include "GameObj.h"

class Entity: public GameObj
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

	// gameObj method
	void render() {};
	
	// update position
	// virtual void update();
};