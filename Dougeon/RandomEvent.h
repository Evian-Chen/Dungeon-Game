/***********************************************************************
? File: RandomEvent.h
? Description: This program is to handle random event in the game play
***********************************************************************/
#pragma once

#include "Position.h"
#include "allConstants.h"
#include "Role.h"

class RandomEvent
{
protected:
	Position ePos;	//position where the event will be triggered
	char icon = EVENT;	//icon of the event

public:
	//constructors
	RandomEvent() {};
	RandomEvent(int x, int y) : ePos(x, y) {};
	RandomEvent(Position p) : ePos(p) {};

	// setter
	void setPos(Position p) { this->ePos = p; }

	// getter
	Position getPos() { return this->ePos; }
	char getIcon() { return this->icon; }
	int getX() { return this->ePos.x; }
	int getY() { return this->ePos.y; }

	void randEvent(Role&);
};