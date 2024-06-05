/***********************************************************************
? File:Enemy.h
? Description: This file contains the definition of the Enemy class which
				inherits from the Entity class. It includes properties relevant to enemy
				characteristics in a game, such as health, attack power, defense, and
				combat-related methods.
***********************************************************************/

#pragma once
#include "Entity.h"
#include "Position.h"
#include "allConstants.h"

class Enemy : public Entity
{
public:
	int round = 0; // Tracks the number of combat rounds this enemy has been involved in

	// Default constructor
	Enemy() : Entity()
	{
		vitality = 30 + rand() % 15;
		speed = 30 + rand() % 25;
		hitRate = 40 + rand() % 20;
		pAttack = 5 + rand() % 10;
		mAttack = 5 + rand() % 10;
		pDefense = rand() % 20;
		mDefense = rand() % 20;
		isDead = false;
		inBattle = true;
		eicon = ENEMY;
	}

	// Constructor that initializes an enemy with a position
	Enemy(Position pos) : Entity(pos)
	{
		vitality = 30 + rand() % 15;
		speed = 30 + rand() % 25;
		hitRate = 40 + rand() % 20;
		pAttack = 5 + rand() % 10;
		mAttack = 5 + rand() % 10;
		pDefense = rand() % 20;
		mDefense = rand() % 20;
		isDead = false;
		inBattle = true;
		eicon = ENEMY;
	}

	// Getter methods
	int getVitality() const { return vitality; }
	int getSpeed() const { return speed; }
	int getHitRate() const { return hitRate; }
	int getPAttack() const { return pAttack; }
	int getMAttack() const { return mAttack; }
	int getPDefense() const { return pDefense; }
	int getMDefense() const { return mDefense; }
	int getIcon() const { return eicon; }
	bool getIsDead()const { return isDead; }
	int getX() { return this->position.x; }
	int getY() { return this->position.y; }
	char getIcon() { return this->eicon; }
	bool getInBattle() { return this->inBattle; }
	Position getPos() { return this->position; }

	// Setter methods
	void setVitality(int value) { this->vitality = value; }
	void setDead(bool value) { this->isDead = value; }
	void setPos(Position p)
	{
		this->position = p;
	}
	void setX(int x) { this->position.x = x; }
	void setY(int y) { this->position.y = y; }
	void setInBattle(bool var) { this->inBattle = var; }

	// Combat functions and variables
	int angryDebuff = 0; // Represents a debuff applied when the enemy is angry
	int attack(int); // Calculates damage dealt by an attack
	bool provoke(int); // Attempts to provoke another character
	int shockBlast(int); // Executes a shock blast attack
	int heal(int); // Heals the enemy

	void showStatus(); // Displays the current status of the enemy
};