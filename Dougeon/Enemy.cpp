#include "Enemy.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Enemy::showStatus()
{
	cout << "|| ¡´ Vitality: " << setw(10) << vitality << '\n'
		<< "|| ¡´ speed: " << setw(13) << speed << '\n'
		<< "|| ¡´ hitRate: " << setw(11) << hitRate << '\n'
		<< "|| ¡´ physical Attack: " << setw(3) << pAttack << '\n'
		<< "|| ¡´ magic Attack: " << setw(6) << mAttack << '\n'
		<< "|| ¡´ physical Defense: " << setw(2) << pDefense << '\n'
		<< "|| ¡´ magic Defense: " << setw(5) << mDefense << '\n';
}

int Enemy::attack()
{
	if (rand() % 100 < hitRate) {
		return pAttack;
	}
	return 0;
}

bool Enemy::provoke()
{
	if (rand() % 100 < (vitality / (100 + pAttack + mAttack))) {
		return true;
	}
	return false;
}

int Enemy::shockBlast()
{
	if (rand() % 100 < (hitRate - 5)) {
		return mAttack * 0.5;
	}
	return 0;
}

int Enemy::heal()
{
	if (rand() % 100 < (hitRate)) {
		return mAttack * 1.5;
	}
	return 0;
}