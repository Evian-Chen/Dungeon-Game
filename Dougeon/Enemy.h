#pragma once
#include "Entity.h"
#include "Position.h"
#include "allConstants.h"
// #include "Skill.h"

class Enemy : public Entity
{
public:
	int round = 0;

	Enemy() : Entity()
	{
		vitality = 1;
		speed = 10;
		hitRate = 98;
		pAttack = 10;
		mAttack = 10;
		pDefense = 10;
		mDefense = 10;
		isDead = false;
		inBattle = true;
		eicon = ENEMY;
	}

	Enemy(Position pos) : Entity(pos)
	{
		vitality = 1;
		speed = 10;
		hitRate = 98;
		pAttack = 10;
		mAttack = 10;
		pDefense = 10;
		mDefense = 10;
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

	// combat fun&variables
	int angryDebuff = 0;
	int attack(int);
	bool provoke(int);
	int shockBlast(int);
	int heal(int);
	// int speedUp();

	void showStatus();
};