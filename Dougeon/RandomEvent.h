#pragma once

#include "Position.h"
#include "allConstants.h"

class RandomEvent
{
protected:
	Position ePos;
	char icon = EVENT;

public:
	RandomEvent() {};
	RandomEvent(int x, int y) : ePos(x, y) {};
	RandomEvent(Position p) : ePos(p) {};

	// setter
	void setPos(Position p) { this->ePos = p; }

	// getter
	Position getPos() { return this->ePos; }
};