#pragma once
#include "Entity.h"
#include "Position.h"
#include "allConstants.h"
// #include "Skill.h"

class Enemy : public Entity
{
public:
	Enemy() : Entity()
	{
		vitality = rand() % 11 + 10;
		speed = rand() % 26 + 30;
		hitRate = rand() % 21 + 40;
		pAttack = rand() % 11 + 5;
		mAttack = rand() % 11 + 5;
		pDefense = rand() % 21;
		pDefense = rand() % 21;
		isDead = false;
		eicon = ENEMY;
	}

	Enemy(Position pos) : Entity(pos)
	{
		position = pos;
		vitality = rand() % 11 + 10;
		speed = rand() % 26 + 30;
		hitRate = rand() % 21 + 40;
		pAttack = rand() % 11 + 5;
		mAttack = rand() % 11 + 5;
		pDefense = rand() % 21;
		pDefense = rand() % 21;
		isDead = false;
		eicon = ENEMY;
	}

	// gameObj method
	void render() {};

	// Getter methods
	int getVitality() const { return vitality; }
	int getSpeed() const { return speed; }
	int getHitRate() const { return hitRate; }
	int getPAttack() const { return pAttack; }
	int getMAttack() const { return mAttack; }
	int getPDefense() const { return pDefense; }
	int getMDefense() const { return mDefense; }
	int getIcon() const { return eicon; }

	// Setter methods
	void setVitality(int value) { vitality = value; }
	void setDead(bool value) { isDead = value; }
	void setPos(Position p)
	{
		this->position = p;
	}
	void setX(int x) { this->position.x = x; }
	void setY(int y) { this->position.y = y; }

	// getter
	int getX() { return this->position.x; }
	int getY() { return this->position.y; }
	char getIcon() { return this->eicon; }

	void attack(Enemy& enemy) {};
	void flee() {};
	void provoke() {};
	void shockBlast() {};
	void heal() {};
	void speedUp() {};

	// undefine!!! from Entity update
	// update position
	// check if enemy has anything to do with role
	void update();
};