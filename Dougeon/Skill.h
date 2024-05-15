#pragma once

class Skill
{
protected:
	int provokeCooldown = 2;
	int shockBlastCooldown = 1;
	int healCooldown = 1;
	int speedUpCooldown = 3;
public:
	// default constructor
	Skill() {}

	// active skills
	virtual void attack() = 0;
	virtual void flee() = 0;
	virtual void provoke() = 0;
	virtual void shockBlast() = 0;
	virtual void heal() = 0;
	virtual void speedUp() = 0;

	// check is cooldown for skills
	bool isProvokeCooldown() { return provokeCooldown > 0; }
	bool isShockBlastCooldown() { return shockBlastCooldown > 0; }
	bool isHealCooldown() { return healCooldown > 0; }
	bool isSpeedUpCooldown() { return speedUpCooldown > 0; }

	// update cooldown (pass-by-reference, should change the specific value)
	void updateCooldown(int& cooldown)
	{
		if (cooldown > 0) { cooldown--; }
	}
};