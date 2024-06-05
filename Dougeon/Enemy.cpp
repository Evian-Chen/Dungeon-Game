/*****************************************************************//**
 * File : Enemy.cpp
 * Description : this progam is to implement the functins declared in enemy class
 *  *********************************************************************/
#include "Enemy.h"
#include <iostream>
#include <iomanip>

using namespace std;

//function to show the status of the enemy by a specific format
void Enemy::showStatus()
{
	cout << "|| ¡´ Vitality: " << setw(10) << this->vitality << '\n'
		<< "|| ¡´ magic Attack: " << setw(6) << this->mAttack << '\n'
		<< "|| ¡´ physical Defense: " << setw(2) << this->pDefense << '\n'
		<< "|| ¡´ magic Defense: " << setw(5) << this->mDefense << '\n'
		<< "|| ¡´ angry debuff: " << setw(5) << this->angryDebuff << '\n';
}

//function when enemy initiate an attack
int Enemy::attack(int hitrate)
{
	// roll one dice according to hit rate
	if (angryDebuff > 0) { hitrate -= 30; angryDebuff--; }	//detuct hit rate if it is angry debuffed
	if (rand() % 100 < hitrate) {
		return pAttack;
	}
	return 0;
}

//functin when enemy use provoke spell
bool Enemy::provoke(int hitrate)
{
	// roll one dice according to hit rate
	if (angryDebuff > 0) { hitrate -= 30; angryDebuff--; }	//detuct hit rate if it is angry debuffed
	if (rand() % 100 < (vitality / (100 + pAttack + mAttack))) {
		return true;
	}
	return false;
}

//function when enemy use shockBlast
int Enemy::shockBlast(int hitrate)
{
	if (angryDebuff > 0) { hitrate -= 30; angryDebuff--; }	//detuct hit rate if it is angry debuffed
	int acc = 0;

	// roll three dice
	for (int i = 0; i < 3; i++) {
		if (rand() % 100 < (hitrate - 5)) {
			acc++;
		}
	}
	return int((mAttack / 2.0) * (acc / 3.0));
}

int Enemy::heal(int hitrate)
{
	if (angryDebuff > 0) { hitrate -= 30; angryDebuff--; }	//detuct hit rate if it is angry debuffed
	int acc = 0;

	// roll two dice
	for (int i = 0; i < 2; i++) {
		if (rand() % 100 < (hitrate)) {
			acc++;
		}
	}
	return int((mAttack / 2 + mAttack) * (acc / 2.0));
}