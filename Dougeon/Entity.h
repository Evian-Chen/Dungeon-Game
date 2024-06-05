/*****************************************************************//**
 * File : Entity.h
 * Description : This program is to declare entity class which is the status of characters of
 *				 hero and enemies, and the class will be inherited by other character classes
 *  *********************************************************************/
#pragma once
#include "Position.h"

class Entity
{
	//status of a character
protected:
	Position position;	//location
	char eicon;	//represented icon
	int vitality, focus = 0, speed, hitRate;	//hp , focus, speed and hitrate
	int pAttack, mAttack, pDefense, mDefense;	//physical \ magical attck and defense 
	bool isDead;	//is the character is dead
	bool inBattle;	//is the character is in battle
public:
	//constructors
	Entity() {};
	Entity(Position pos) :position(pos.x, pos.y) {};
	Entity(int startX, int startY) : position(startX, startY) {};
};