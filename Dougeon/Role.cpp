#include "Role.h"
#include <iostream>
// #include "Enemy.h"
// #include "allConstants.h"

using namespace std;

int Role::money = 600;


/*
void Role::attack(Enemy& enemy)
{
	int cur = enemy.getVitality();
	enemy.setVitality(cur - pAttack);
	if (enemy.getVitality() <= 0)
	{
		enemy.setDead(true);
	}
}

void Role::flee()
{
	inBattle = false;
}

void Role::provoke()
{
	if (!isProvokeCooldown())
	{
		int a;
		// can provoke
		// give enemy debuff::angry, last 3 rounds
	}
	else { updateCooldown(provokeCooldown); }
}

void Role::shockBlast(Enemy& enemy)
{
	if (!isShockBlastCooldown())
	{
		int cur = enemy.getVitality();
		enemy.setVitality(cur - pAttack);
		if (enemy.getVitality() <= 0)
		{
			enemy.setDead(true);
		}
	}
	else { updateCooldown(shockBlastCooldown); }
}

void Role::heal()
{
	if (!isHealCooldown())
	{
		setVitality(100);
	}
	else { updateCooldown(healCooldown); }
}

void Role::speedUp()
{
	if (!isProvokeCooldown())
	{
		int a;
		// can speedUp
		// give self or partner buff::speedup, last one round
	}
	else { updateCooldown(provokeCooldown); }
}

*/


void Role::updatePack(string item)
{
	equip.push_back(item);
	avalEquip[item] = true;
}

void Role::showBag()
{
	if (equip.size() == 0)
	{
		cout << "There's nothing in your bag.\n";
	}
	else
	{
		for (const std::string& str : equip) {
			cout << str << endl;
		}
	}
	cout << "money: " << money;
}

void Role::showStatus()
{
	cout << "Vitality: " << vitality << '\n'
		 << "focus: " << focus << '\n'
		 << "speed: " << speed << '\n'
		 << "hitRate: " << hitRate << '\n'
		 << "physical Attack: " << pAttack << '\n'
		 << "magic Attack: " << mAttack << '\n'
		 << "physical Defense: " << pDefense << '\n'
		 << "magic Defense: " << mDefense << '\n';
}