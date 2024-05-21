#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Shop.h"
// #include "Skill.h"
#include "Equipment.h"
#include "Position.h"
#include <iostream>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include "allConstants.h"
// #include "allConstants.h"

using namespace std;

class Role : public Entity, public Equipment, public Shop
{
private:  
	// define if role has the equipment to use skill
	bool hasProvoke = false;
	bool hasShockBlast = false;
	bool hasHeal = false;
	bool hasSpeedUp = false;

	// number of activated equipments
	int actiEquip = 0;

	int avalstep = 0;
	int maxMovePoint = 0;
	double diceAccRate = 0;

	// name of all avilable equipments
	vector<string> equip = {};

	// equipments and their amount
	map<string, int> equipNum;

	// store if the equipment is activated
	// if there are more than one same equipment
	// one acitvated implies all activated 
	map<string, bool> avalEquip = { { "woodenSword", false },
								    { "hammer", false },
								    { "giantHammer", false },
								    { "woodenShield", false },
								    { "plateArmor", false },
								    { "robe", false },
								    { "holyGrail", false },
								    { "shoes", false },
								    { "godsbeard", false },
								    { "teleportScroll", false } };

	// activated equipments and their amount
	map<string, int> avalEquipNum;

public:
	static int money;

	// item
	static int godsBeard;
	static int goldenRoot;

	Role() {};

	// default constructor
	Role(Position p) : Entity(p), Equipment()
	{
		eicon = PLAYER_ICON;
		vitality = 100;
		focus = 3;
		speed = 75;
		hitRate = rand() % 21 + 40;
		pAttack = rand() % 11 + 5;
		mAttack = rand() % 11 + 5;
		pDefense = rand() % 21;
		mDefense = rand() % 21;
		isDead = false;
		inBattle = false;
		maxMovePoint = speed / 10;
		diceAccRate = min(0.9, ((float)speed / 100.0));
	}

	// default constructor
	Role(int startX, int startY) : Entity(startX, startY), Equipment()
	{
		eicon = PLAYER_ICON;
		vitality = rand() % 16 + 30;
		focus = 3;
		speed = rand() % 26 + 30;
		hitRate = rand() % 21 + 40;
		pAttack = rand() % 11 + 5;
		mAttack = rand() % 11 + 5;
		pDefense = rand() % 21;
		mDefense = rand() % 21;
		isDead = false;
		inBattle = false;
		maxMovePoint = speed / 10;
		diceAccRate = min(0.9, ((float)speed / 100.0));
	}

	// void setPos(Position p) { pos = p; }

	// get role's position
	Position getPosition() { return position; }

	// gameObj method
	void render() {};

	// Setter
	void setHasProvoke() { hasProvoke = true; }
	void setHasShockBlast() { hasShockBlast = true; }
	void setHasHeal() { hasHeal = true; }
	void setHasSpeedUp() { hasSpeedUp = true; }
	void setInBattle() { inBattle = true; }
	void setVitality(int value) { vitality = value; }

	// Getter
	int getVitality() const { return vitality; }
	int getFocus() const { return focus; }
	int getSpeed() const { return speed; }
	int getHitRate() const { return hitRate; }
	int getPAttack() const { return pAttack; }
	int getMAttack() const { return mAttack; }
	int getPDefense() const { return pDefense; }
	int getMDefense() const { return mDefense; }

	/*
	void attack(Enemy& enemy);
	void flee();
	void provoke();
	void shockBlast(Enemy& enemy);
	void heal();
	void speedUp();	
	*/


	// move role
	void move(const Position& delta);
	
	void showBag();
	void chooseEquip();
	void showChoosen(int, int);

	void showStatus();

	void showShop();
	void showChoosenItem(int, int, map<string, int>&);
	bool buyItem(int, map<string, int>&);

	void throwDice();
	void showRollDice(int);
	int getAvalStep() { return avalstep; }
};