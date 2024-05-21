#include "Role.h"
#include "Position.h"
#include "allConstants.h"
#include <utility>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <vector>

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
	Position temp = position + delta;

	// prevent role from going put of the range of gMap
	if (temp.isPositionValid())
	{
		if (avalstep == 0)
		{
			// step limit, time to throw the dice
			throwDice();
		}
		else
		{
			position += delta;
			avalstep--;
		}
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
	else if (temp.isOnEnemy())
	{
		// ready to combat
		int a;
	}
}

void Role::showShop()
{
	map<string, int> bought;

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

			// use red to emphasize the player is so poor that con't but this item
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			WORD colorSettings = FOREGROUND_RED;
			SetConsoleTextAttribute(hConsole, colorSettings);
			if (!canBuy) { cout << "You don't have enough money\n"; }
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
		}
		else if (GetAsyncKeyState(0x45)) { // click E to exit
			break;
		}

		// prevent high CPU usage
		system("pause");
		// Sleep(100);
	}
}

void Role::showChoosenItem(int num, int maxWidth, map<string, int>& bought)
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

	cout << "Item" << setw(maxWidth - 4) << "Amount" << endl;

	int total = 0;  // caculate the total spend

	if (bought.size() != 0)
	{
		for (auto& pair: bought)
		{
			cout << pair.first << setw(maxWidth - pair.first.length());
			cout << pair.second << endl;
			total += pair.second * priceTag[pair.first];
		}
		cout << "Total spent: " << total << endl;
	}
	else
	{
		cout << "You haven't bought anything.\n\n";
	}

	cout << "\n/** Click E to exit. **/\n" << endl;

}

bool Role::buyItem(int numOfEquip, map<string, int>& bought)
{
	string item = itemsName[numOfEquip].first;

	// money is enough and the hasn't bouhgt the item yet
	if (itemsName[numOfEquip].second <= money)
	{
		// spend money
		money -= itemsName[numOfEquip].second;

		// add item to bag
		if (this->equipNum.find(item) == this->equipNum.end())
		{
			this->equipNum[item] = 1;
			equip.push_back(item);
			bought[item] = 1;
		}
		else
		{
			this->equipNum[item]++;
			bought[item]++;
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
		cout << "There's nothing in your bag.\n\n";
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
	size_t maxWidth = 0;
	for (const string& str : equip) {
		maxWidth = max(maxWidth, str.length());
	}
	maxWidth *= 2;

	showChoosen(numOfEquip, maxWidth);

	char input = _getch();

	while (input != 'b') {
		if (input == 80 && numOfEquip + 1 < equip.size()) { // Check if the up arrow key is pressed
			numOfEquip++;
			showChoosen(numOfEquip, maxWidth);
		}
		else if (input == 72 && numOfEquip - 1 >= 0) {
			numOfEquip--;
			showChoosen(numOfEquip, maxWidth);
		}
		else if (input == '+' || input == 78) // click + to activate the equipment
		{ 	
			// this equipment is enough to use
			if (equipNum[equip[numOfEquip]] > 0 && actiEquip < ACTEQUIPLIM)
			{
				// activate the equipmeant
				avalEquip[equip[numOfEquip]] = true;
				if (avalEquipNum.find(equip[numOfEquip]) == avalEquipNum.end()) { avalEquipNum[equip[numOfEquip]] = 1; }
				else{ avalEquipNum[equip[numOfEquip]]++; }
				equipNum[equip[numOfEquip]]--;

				actiEquip++;
			}
			
			showChoosen(numOfEquip, maxWidth);
		}
		else if (input == '-' || input == 74) // click - to un-activate the equipment
		{ 
			// the equipment can be found in avalEquipNum
			if (avalEquipNum.find(equip[numOfEquip]) != avalEquipNum.end())
			{
				avalEquipNum[equip[numOfEquip]]--;
				if (equipNum.find(equip[numOfEquip]) == equipNum.end()) { equipNum[equip[numOfEquip]] = 1; }
				else { equipNum[equip[numOfEquip]]++; }

				actiEquip--;

				if (avalEquipNum[equip[numOfEquip]] == 0)
				{
					avalEquip[equip[numOfEquip]] = false;
					avalEquipNum.erase(equip[numOfEquip]);
				}
			}

			showChoosen(numOfEquip, maxWidth);
		}
		else
		{
			showChoosen(numOfEquip, maxWidth);
		}

		input = _getch();
	}
}

void Role::showChoosen(int num, int maxWidth)
{
	system("cls");  // clear console

	cout << "/** Click enter to activate quipments.          **/" << endl;
	cout << "/** Use up and down arrows to choose quipments. **/\n" << endl;
	cout << "\n======== Your equipments. ========\n" << endl;
	cout << "Equipment" << setw(maxWidth - 9) << "Amount" << endl;

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
		cout << equip[i] << setw(maxWidth - equip[i].length());
		cout << equipNum[equip[i]] << endl;
		// cout << equip[i] << endl;
	}

	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	cout << "\n======== The following is choosen equipments. ========\n" << endl;
	cout << "Equipment" << setw(maxWidth * 2 - 9) << "Amount" << endl;

	for (auto& pair : avalEquipNum)
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
			cout << " is active." << setw(maxWidth * 2 - pair.first.length() - 14) << pair.second << "X\n";
		}
	}
	if (actiEquip == ACTEQUIPLIM)
	{
		colorSettings = FOREGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << "Your equipments have reached the limit.\n\n";
		colorSettings = FOREGROUND_INTENSITY;
		SetConsoleTextAttribute(hConsole, colorSettings);
	}

	cout << "\n======== Your wallet. ========\n" << endl;
	cout << "money: " << money << endl;

	cout << "\n/** Click B to exit. **/\n" << endl;
}

void Role::showStatus()
{
	cout << "||===== Your status =====||\n\n";
	cout << "|| ¡´ Vitality: " << setw(10) << vitality << '\n'
		<< "|| ¡´ focus: " << setw(13) << focus << '\n'
		<< "|| ¡´ speed: " << setw(13) << speed << '\n'
		<< "|| ¡´ hitRate: " << setw(11) << hitRate << '\n'
		<< "|| ¡´ physical Attack: " << setw(3) << pAttack << '\n'
		<< "|| ¡´ magic Attack: " << setw(6) << mAttack << '\n'
		<< "|| ¡´ physical Defense: " << setw(2) << pDefense << '\n'
		<< "|| ¡´ magic Defense: " << setw(5) << mDefense << '\n';
}

void Role::throwDice()
{
	cout << "||=====================================||\n";
	cout << "||=== It's your turn to roll dice! ====||\n";
	cout << "||=====================================||\n\n\n";
	cout << "|| ¡´ Do you want to use focus? (Y/N): ";

	char input = _getch();
	if (input == 'Y')
	{
		cout << input << "\n|| ¡´ How many points of focus you want to use?\n|| ¡´ Enetr here: ";
		input = _getch();
		int useFocus = input - '0';
		cout << useFocus << endl;

		while (useFocus > focus)
		{
			cout << "You don't have enough focus, enter again or enter 0 to continue: ";
			input = _getch();
			useFocus = input - '0';
		}
		
		focus -= useFocus;
		cout << "|| ¡´ You use " << input << " focus, press Enter to start rolling: ";
		
		while (input != '\r')
		{
			input = _getch();
		}
		showRollDice(useFocus);
	}
	else if (input == 'N')
	{
		cout << input << "\nPress Enter to start rolling! \n";

		while (input != '\n')
		{
			input = _getch();
		}
		showRollDice(0);
	}
}

void rollingAnime()
{
	cout << "\n\n||=== generating the result... ====||\n";
	cout << "|| ";

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED;
	SetConsoleTextAttribute(hConsole, colorSettings);
	for (int i = 0; i < 31; i++)
	{
		cout << ' ';
		Sleep(100);
	}

	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	cout << " ||\n\n";
}

void Role::showRollDice(int useFocus)
{
	string dice = "";
	double chance = 0;
	for (int i = 0; i < useFocus; i++)
	{
		dice += "T";
		avalstep++;
	}

	srand(time(0));
	for (int i = 0; i < maxMovePoint - useFocus; i++)
	{
		chance = (rand() % 100) / 100.0;
		if (chance < diceAccRate) { dice += "T"; avalstep++; }
		else { dice += "F"; }
	}

	rollingAnime();

	cout << "Here's the result: ";

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED;
	SetConsoleTextAttribute(hConsole, colorSettings);

	cout << dice << endl;

	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);
	cout << "\n\nYou have " << avalstep << " steps in this round.\n";

	cout << "\n/** Click any key to exit. **/\n";
	chance = _getch();
}

