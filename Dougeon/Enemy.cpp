#include "Enemy.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Enemy::showStatus()
{
	cout << "|| �� Vitality: " << setw(10) << vitality << '\n'
		<< "|| �� speed: " << setw(13) << speed << '\n'
		<< "|| �� hitRate: " << setw(11) << hitRate << '\n'
		<< "|| �� physical Attack: " << setw(3) << pAttack << '\n'
		<< "|| �� magic Attack: " << setw(6) << mAttack << '\n'
		<< "|| �� physical Defense: " << setw(2) << pDefense << '\n'
		<< "|| �� magic Defense: " << setw(5) << mDefense << '\n'
		<< "|| �� angry debuff: " << setw(5) << angryDebuff << '\n';
}

int Enemy::attack(int hitrate)
{
	// roll one dice
	if (angryDebuff > 0) { hitrate -= 30; angryDebuff--; }
	if (rand() % 100 < hitrate) {
		return pAttack;
	}
	return 0;
}

bool Enemy::provoke(int hitrate)
{
	// roll one dice
	if (angryDebuff > 0) { hitrate -= 30; angryDebuff--; }
	if (rand() % 100 < (vitality / (100 + pAttack + mAttack))) {
		return true;
	}
	return false;
}

int Enemy::shockBlast(int hitrate)
{
	// roll three dice
	if (angryDebuff > 0) { hitrate -= 30; angryDebuff--; }
	int acc = 0;
	for (int i = 0; i < 3; i++) {
		if (rand() % 100 < (hitrate - 5)) {
			acc++;
		}
	}
	return int((mAttack / 2.0) * (acc / 3.0));
}

int Enemy::heal(int hitrate)
{
	// roll two dice
	if (angryDebuff > 0) { hitrate -= 30; angryDebuff--; }
	int acc = 0;
	for (int i = 0; i < 2; i++) {
		if (rand() % 100 < (hitrate)) {
			acc++;
		}
	}
	return int((mAttack / 2 + mAttack) * (acc / 2.0));
}