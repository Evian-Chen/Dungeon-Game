#include "Role.h"
#include "Position.h"
#include <utility>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
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

void Role::move(const Position& delta) {
	Position temp = pos + delta;

	// prevent role from going put of the range of gMap
	if (temp.isPositionValid())
	{
		pos += delta;
	}
	else if (temp.isOnShop())
	{
		cout << "======== Do you want to go to the shop? (Y/N) ========\nY/N:";
		char input = _getch();
		if (input == 'Y')
		{
			cout << input << "\nClick enter to go into the shop! :D\n";
			input = _getch();
			if (input == '\n') {};
			showShop();
		}
		else if (input == 'N')
		{
			cout << input << "\nOkay, then you can click enter to get out. >:(\n";
			input = _getch();
			if (input == '\n') {};
		}
		else
		{
			cout << "Invalid input, enter again.\n";
		}
	}
}

void Role::showShop()
{
	size_t maxWidth = 0;
	for (const auto& pair : itemsName) {
		maxWidth = max(maxWidth, pair.first.size());
	}
	maxWidth *= 2;

	int numOfEquip = 0;
	showChoosenItem(numOfEquip, maxWidth);

	while (true) {
		// Check if the up arrow key is pressed
		if (GetAsyncKeyState(VK_DOWN) && numOfEquip + 1 < itemsName.size()) {
			numOfEquip++;
			showChoosenItem(numOfEquip, maxWidth);
		}
		else if (GetAsyncKeyState(VK_UP) && numOfEquip - 1 >= 0) {
			numOfEquip--;
			showChoosenItem(numOfEquip, maxWidth);
		}
		else if (GetAsyncKeyState(0x45)) {
			break;
		}

		// Add a delay to prevent high CPU usage
		Sleep(100);
	}
}

void Role::showChoosenItem(int num, int maxWidth)
{
	system("cls");  // clear console

	cout << "\n======== Your wallet. ========\n" << endl;
	cout << "money: " << money << endl << endl;

	cout << "/** Click enter to buy items.               **/" << endl;
	cout << "/** Use up and down arrows to choose items. **/\n" << endl;
	cout << "\n======== Items in sold. ========\n" << endl;
	cout << "Item" << setw(maxWidth - 4) << "Price" << endl;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings |= BACKGROUND_INTENSITY;

	// print out choosen equip
	for (int i=0; i < itemsName.size(); i++)
	{
		// turn the choosen equip to red word
		if (i == num)
		{
			colorSettings = FOREGROUND_RED;
		}
		else
		{
			colorSettings = FOREGROUND_INTENSITY;
		}

		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << itemsName[i].first << setw(maxWidth - itemsName[i].first.length());
		cout << itemsName[i].second << endl;
	}

	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	cout << "\n/** Click E to exit. **/\n" << endl;

}

void Role::butItem()
{
	showBag();
}

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
		chooseEquip();
	}
}

void Role::chooseEquip()
{
	int numOfEquip = 0;
	showChoosen(numOfEquip);

	while (true) {
		// Check if the up arrow key is pressed
		if (GetAsyncKeyState(VK_DOWN) && numOfEquip + 1 < equip.size()) {
			numOfEquip++;
			showChoosen(numOfEquip);
		}
		else if (GetAsyncKeyState(VK_UP) && numOfEquip - 1 >= 0) {
			numOfEquip--;
			showChoosen(numOfEquip);
		}
		else if (GetAsyncKeyState(VK_RETURN)) {
			avalEquip[equip[numOfEquip]] = !avalEquip[equip[numOfEquip]];
			showChoosen(numOfEquip);
		}
		else if (GetAsyncKeyState(0x45)) {
			break;
		}

		// Add a delay to prevent high CPU usage
		Sleep(100);
	}
}

void Role::showChoosen(int num)
{
	system("cls");  // clear console

	cout << "/** Click enter to activate quipments.          **/" << endl;
	cout << "/** Use up and down arrows to choose quipments. **/\n" << endl;
	cout << "\n======== Your equipments. ========\n" << endl;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings |= BACKGROUND_INTENSITY;

	// print out choosen equip
	for (int i = 0; i < equip.size(); i++)
	{
		// turn the choosen equip to red word
		if (i == num)
		{
			colorSettings = FOREGROUND_RED;
		}
		else
		{
			colorSettings = FOREGROUND_INTENSITY;
		}

		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << equip[i] << endl;
	}

	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	cout << "\n======== The following is choosen equipments. ========\n" << endl;

	for (auto& pair : avalEquip)
	{
		if (pair.second)
		{
			// hightlight the skill
			colorSettings = BACKGROUND_GREEN; 
			colorSettings |= BACKGROUND_RED;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << pair.first;

			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << " is active.\n";
		}
	}

	cout << "\n======== Your wallet. ========\n" << endl;
	cout << "money: " << money << endl;

	cout << "\n/** Double click E to exit. **/\n" << endl;
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