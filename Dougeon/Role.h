#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Shop.h"
// #include "Skill.h"
#include "Position.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <cstdlib> // For rand() and srand()
#include "allConstants.h"
// #include "allConstants.h"

using namespace std;

class Role : public Entity, public Shop
{
private:  
	// define if role has the equipment to use skill
	bool hasProvoke = false;
	bool hasShockBlast = false;
	bool hasHeal = false;
	bool hasSpeedUp = false;

	// number of activated equipments
	int actiEquip = 0;

	int avalstep = 100;
	int maxMovePoint = 0;
	double diceAccRate = 0;

	
	// name of all avilable equipments
	vector<string> equip = {};

	// equipments and their amount
	map<string, int> equipNum;

	// store if the equipment is activated
	// if there are more than one same equipment
	// one acitvated implies all activated 
	map<string, bool> avalEquip = { 
		{ "woodenSword", false },
		{ "giantHammer", false },
		{ "ritualSword", false },
		{ "woodenShield", false },
		{ "plateArmor", false },
		{ "robe", false },
		{ "holyGrail", false },
		{ "other", true } }; // other is for attack/flee (default)

	// activated equipments and their amount
	map<string, int> avalEquipNum;

public:
	static int money;

	// item
	static int godsBeard;
	static int goldenRoot;
	// static int teleportScroll;

	// during combat
	int round = 0;
	string curSkill = "";

	Role() {};

	// default constructor
	Role(Position p) : Entity(p)
	{
		eicon = PLAYER_ICON;
		vitality = 100;
		focus = 100;
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
	Role(int startX, int startY) : Entity(startX, startY)
	{
		eicon = PLAYER_ICON;
		vitality = 100;
		focus = 100;
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

	// void setPos(Position p) { pos = p; }

	// get role's position
	Position getPosition() { return position; }

	// Setter
	void setHasProvoke() { hasProvoke = true; }
	void setHasShockBlast() { hasShockBlast = true; }
	void setHasHeal() { hasHeal = true; }
	void setHasSpeedUp() { hasSpeedUp = true; }
	void setInBattle(bool var) { inBattle = var; }
	void setVitality(int value) { vitality = value; }
	void setFocus(int value) { focus = value; }
	void setHitRate(int value) { hitRate = value; }
	void setSpeed(int value) { speed = value; }
	void setDead(bool var) { isDead = var; }

	// Getter
	int getVitality() const { return vitality; }
	int getFocus() const { return focus; }
	int getSpeed() const { return speed; }
	int getHitRate() const { return hitRate; }
	int getPAttack() const { return pAttack; }
	int getMAttack() const { return mAttack; }
	int getPDefense() const { return pDefense; }
	int getMDefense() const { return mDefense; }
	bool getInBattle()const { return inBattle; }
	bool getIsDead() { return this->isDead; }

	
	void attack(int, vector<Enemy*>&, int, int, int, HANDLE, WORD);
	void provoke(int, vector<Enemy*>&, int, HANDLE, WORD);
	void hammerSplash(vector<Enemy*>&, int, int);
	// void fortify();
	void shockBlast(vector<Enemy*>&, int, int, int, HANDLE, WORD);
	void heal(int, HANDLE, WORD);
	void speedUp(int, HANDLE, WORD);
	bool flee();
	
	void woodenSword() { pAttack += 5;  hitRate += 10; }
	void hammer(){ pAttack += 15; hitRate -= 15;}
	void giantHammer(){ pAttack += 20; hitRate -= 15; }
	void woodenShield(){ pDefense += 10; }
	void robe(){ mDefense += 10; }
	void holyGrail(){ mDefense += 30; }
	void ritualSword() { mAttack += 15; }
	void plateArmor() { pDefense += 2; speed -= 10; }

	// variable for combat
	int numSkill = 0;
	vector<string> skillVec = { "attack","flee","provoke","shockBlast","heal","speedUp" };
	map<string, string> skillEquipMap = { 
		{"attack","other"},
		{"flee","other"},
		{"provoke","woodenShield"},
		{"shockBlast","ritualSword"}, 
		{"heal","holyGrail"},
		{"speedUp", "woodenSword"} };
	map<string, void (Role::*)()> equipFuncList = {
		{ "woodenSword", &Role::woodenSword },
		{ "ritualSword", &Role::ritualSword },
		{ "giantHammer", &Role::giantHammer },
		{ "woodenShield", &Role::woodenShield },
		{ "PlateArmor", &Role::plateArmor },
		{ "robe", &Role::robe },
		{ "holyGrail", &Role::holyGrail }
	};

	// for cool down skill/item/buff
	int speedUpCool = 0;
	int angryDebuff = 0;

	// move role
	bool move(const Position& delta);
	
	void showBag();
	void chooseEquip();
	void showChoosen(int, int);
	void teleportScroll();

	void showStatus();

	void showShop();
	void showChoosenItem(int, int, map<string, int>&);
	bool buyItem(int, map<string, int>&);

	void throwDice();
	void showRollDice(int);
	int getAvalStep() { return avalstep; }

	bool combat();
	void showAllStatus(vector<Enemy*>&);
	bool chooseSkill();
	void chooseItem();
	int chooseEnemy(vector<Enemy*>&);
	void attackEnemy(vector<Enemy*>&, int, int);
};