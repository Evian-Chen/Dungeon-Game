#include "Role.h"
#include "Position.h"
#include <utility>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <vector>
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
	vector<string> bought;

	size_t maxWidth = 0;
	for (const auto& pair : itemsName) {
		maxWidth = max(maxWidth, pair.first.size());
	}
	maxWidth *= 2;

	int numOfEquip = 0;
	bool canBuy = false;
	showChoosenItem(numOfEquip, maxWidth, bought);

	while (true) {
		// Check if the up arrow key is pressed
		if (GetAsyncKeyState(VK_DOWN) && numOfEquip + 1 < itemsName.size()) {
			numOfEquip++;
			showChoosenItem(numOfEquip, maxWidth, bought);
		}
		else if (GetAsyncKeyState(VK_UP) && numOfEquip - 1 >= 0) {
			numOfEquip--;
			showChoosenItem(numOfEquip, maxWidth, bought);
		}
		else if (GetAsyncKeyState(VK_SPACE)) {
			canBuy = buyItem(numOfEquip, bought);
			showChoosenItem(numOfEquip, maxWidth, bought);
			if (!canBuy) { cout << "You don't have enough money\n"; }
		}
		else if (GetAsyncKeyState(0x45)) {
			break;
		}

		// prevent high CPU usage
		system("pause");
		// Sleep(100);
	}
}

void Role::showChoosenItem(int num, int maxWidth, vector<string>& bought)
{
	system("cls");  // clear console

	cout << "\n======== Your wallet. ========\n" << endl;
	cout << "money: " << money << endl << endl;

	cout << "/** Click space to buy items.               **/" << endl;
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

	cout << "\n======== Bought items: ========\n";
	if (bought.size() != 0)
	{
		for (string str : bought)
		{
			cout << str << endl;
		}
	}
	else
	{
		cout << "You haven't bought anything.\n";
	}

	cout << "\n/** Click E to exit. **/\n" << endl;

}

bool Role::buyItem(int numOfEquip, vector<string>& bought)
{
	string item = itemsName[numOfEquip].first;

	// money is enough and the hasn't bouhgt the item yet
	if (itemsName[numOfEquip].second <= money)
	{
		if (find(bought.begin(), bought.end(), item) == bought.end())
		{
			// add item to the bag
			money -= itemsName[numOfEquip].second;
			this->equip.push_back(item);
			bought.push_back(item);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void Role::showBag()
{
	if (equip.size() == 0)
	{
		cout << "There's nothing in your bag.\n";
		cout << "Click B to exit.\n";

		// wait for input
		char ch = _getch();
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

	char input = _getch();

	while (input != 'b') {
		if (input == 80 && numOfEquip + 1 < equip.size()) { // Check if the up arrow key is pressed
			numOfEquip++;
			showChoosen(numOfEquip);
		}
		else if (input == 72 && numOfEquip - 1 >= 0) {
			numOfEquip--;
			showChoosen(numOfEquip);
		}
		else if (input == '\n' || input == '\r') { // enter to activate the equipment
			avalEquip[equip[numOfEquip]] = !avalEquip[equip[numOfEquip]];
			showChoosen(numOfEquip);
		}
		else
		{
			showChoosen(numOfEquip);
		}

		input = _getch();
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

	cout << "\n/** Double click B to exit. **/\n" << endl;
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
