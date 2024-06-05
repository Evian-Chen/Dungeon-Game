/***********************************************************************
? File: Role.h
? Description:This file contains the declaration of the Role class, which
				represents a character in the game. The Role class includes methods for
				combat, movement, and interaction with game elements like enemies and shops.
				It inherits from both the Entity and Shop classes, integrating functionalities
				like position management and item transactions.
***********************************************************************/

#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Shop.h"
#include "Position.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <cstdlib> // For rand() and srand()
#include "allConstants.h"

using namespace std;

// The Role class represents the player character in the game,
// Inheriting properties and methods from both Entity and Shop classes.
class Role : public Entity, public Shop
{
private:
	// Flags indicating if the Role has specific skills available.
	bool hasProvoke = false;
	bool hasShockBlast = false;
	bool hasHeal = false;
	bool hasSpeedUp = false;

	// Number of active equipments currently in use by the Role.
	int actiEquip = 0;

	int avalstep = 10;
	int maxMovePoint = 0;
	double diceAccRate = 0;


	// Current available steps in the game logic, primarily for movement.
	vector<string> equip = {};

	// Equipments and their amount
	map<string, int> equipNum;

	// Store if the equipment is activated
	// If there are more than one same equipment
	// One acitvated implies all activated 
	map<string, bool> avalEquip = {
		{ "woodenSword", false },
		{ "giantHammer", false },
		{ "ritualSword", false },
		{ "woodenShield", false },
		{ "plateArmor", false },
		{ "robe", false },
		{ "holyGrail", false },
		{ "other", true } }; // other is for attack/flee (default)

	// Tracks the number of each type of activated equipment.
	map<string, int> avalEquipNum;

public:
	// Static variables for shared game properties like money and dice limits.
	static int money;
	static int diceLimit;

	// Static items that can impact gameplay, like godsBeard and goldenRoot.
	static int godsBeard;
	static int goldenRoot;

	// Combat-specific attributes.
	int round = 0; // Current round number in combat.
	string curSkill = ""; // Currently selected skill to use in combat

	// Constructors
	Role() {};

	// Default constructor
	Role(Position p) : Entity(p)
	{
		eicon = PLAYER_ICON;
		vitality = 30 + rand() % 15;
		focus = 3;
		speed = 30 + rand() % 25;
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

	// Default constructor
	Role(int startX, int startY) : Entity(startX, startY)
	{
		eicon = PLAYER_ICON;
		vitality = 30 + rand() % 15;
		focus = 3;
		speed = 30 + rand() % 25;
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

	// Get role's position
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
	void setPA(int var) { pAttack = var; }
	void setPD(int var) { pDefense = var; }
	void setMA(int var) { mAttack = var; }
	void setMD(int var) { mDefense = var; }

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
	void hammerSplash(vector<Enemy*>&, int, int, HANDLE, WORD);
	void shockBlast(vector<Enemy*>&, int, int, int, HANDLE, WORD);
	void heal(int, HANDLE, WORD);
	int speedUp(int, HANDLE, WORD);
	bool flee();

	// Equipment-related methods
	void woodenSword() { this->pAttack += 5; this->hitRate += 10; }
	void giantHammer() { this->pAttack += 20; this->hitRate -= 15; }
	void woodenShield() { this->pDefense += 10; }
	void robe() { this->mDefense += 10; }
	void holyGrail() { this->mDefense += 30; }
	void ritualSword() { this->mAttack += 15; }
	void plateArmor() { this->pDefense += 2; this->speed -= 10; }

	// Variable for combat
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

	// For cool down skill/item/buff
	int speedUpCool = 0;
	int angryDebuff = 0;

	// Move role
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

	// Methods for game mechanics like moving, shopping, choosing equipment, and combat
	bool combat();
	void showAllStatus(vector<Enemy*>&);
	bool chooseSkill();
	void chooseItem();
	int chooseEnemy(vector<Enemy*>&);
	void attackEnemy(vector<Enemy*>&, int, int);
};