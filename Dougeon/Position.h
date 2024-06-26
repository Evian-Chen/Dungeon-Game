/*****************************************************************//**
 * File : Position.h
 * Description : This program is to implement position class, used by every character, enemies, shop or other objects in the map
 *  *********************************************************************/
#pragma once
#include <iostream>
#include "allConstants.h"

class Position {
public:
	int x, y; // X, y Coordinate

	//constructor
	Position() : x(0), y(0) {}
	Position(int x, int y) : x(x), y(y) {}
	Position(const Position& ref) { *this = ref; }

	//getter of position x
	int getX() {
		return x;
	}

	//getter of position x
	int getY() {
		return y;
	}

	// for checking if the position is valid
	bool isPositionValid()
	{
		// Check whether it is an empty space
		if (this->x < MAP_WIDTH && this->x >= 0 && this->y < MAP_HEIGHT && this->y >= 0 && gMap[this->y][this->x] == ROAD)
			return true;

		return false;
	}

	//function to check if enemy is in the position
	bool isOnEnemy()
	{
		// Check whether it is an empty space
		if (gMap[this->y][this->x] == ENEMY)
			return true;

		return false;
	}

	//function to check if hot spring is in the position
	bool isOnHotSpring()
	{
		// Check whether it is an empty space
		if (gMap[this->y][this->x] == HOTSPRING)
			return true;

		return false;
	}

	//function to check if shop is in the position
	bool isOnShop()
	{
		// Check whether it is an empty space
		if (gMap[this->y][this->x] == SHOP)
			return true;

		return false;
	}

	//function to check if random event is in the position
	bool isOnRandom()
	{
		// Check whether it is an empty space
		if (gMap[this->y][this->x] == EVENT)
			return true;

		return false;
	}

	//function to check if final exit is in the position
	bool isOnFinal()
	{
		// Check whether it is an empty space
		if (gMap[this->y][this->x] == FINAL)
			return true;

		return false;
	}

	// Operator overloading
	Position operator+(const Position& rhs) const {
		return Position(x + rhs.x, y + rhs.y);
	}
	Position& operator+=(const Position& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Position operator-(const Position& rhs) const {
		return Position(x - rhs.x, y - rhs.y);
	}
	Position& operator-=(const Position& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	Position operator*(int scale) const {
		return Position(x * scale, y * scale);
	}
	bool operator==(const Position& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Position& rhs) const {
		return !(*this == rhs);
	}

	friend std::ostream& operator<<(std::ostream& oStream, const Position& pos) {
		return oStream << pos.x << " " << pos.y;
	}
	friend std::istream& operator>>(std::istream& iStream, Position& pos) {
		return iStream >> pos.x >> pos.y;
	}
};