#include "Role.h"
#include "Enemy.h"
#include "RandomEvent.h"
#include "Position.h"
#include "allConstants.h"
#include <utility>
#include <algorithm>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

int Role::money = 600;
int Role::godsBeard = 3;
int Role::goldenRoot = 3;

RandomEvent random;
vector<Enemy*> enemy;

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

void Role::move(const Position& delta) {
	Position temp = position + delta;

	if (avalstep == 0)
	{
		// step limit, time to throw the dice
		throwDice();
	}
	else
	{
		// prevent role from going put of the range of gMap
		if (temp.isPositionValid())
		{
			position += delta;
			avalstep--;
		}
		else if (temp.isOnShop())
		{
			cout << "======== Do you want to go to the shop? (Y/N) ========\nY/N:";
			char input = _getch();
			if (input == 'Y')
			{
				cout << input << "\nClick enter to go into the shop! :D\n";
				input = _getch();
				showShop();
			}
			else if (input == 'N')
			{
				cout << input << "\nOkay, then you can click any key to get out. >:(\n";
				input = _getch();
			}
		}
		else if (temp.isOnHotSpring())
		{
			cout << "======== Do you want to take a rest? (risk 1 foucs to regen 30 health) (Y/N) ========\nY/N:";
			char input = _getch();
			if (input == 'Y')
			{
				cout << input << endl;
				if (focus < 1) {
					cout << "\nYou don't have enough focus.\n";
					cout << "Click enter to continue.\n";
					input = _getch();
					if (input == '\n') {};
				}
				else {
					focus -= 1;
					vitality += 30;
				}
			}
			else if (input == 'N')
			{
				cout << input << "\nOkay, then you can click any key to get out. >:(\n";
				input = _getch();
			}
		}
		else if (temp.isOnEnemy())
		{
			if (!combat()) { // if player flee, then combat returns true
				gMap[temp.getY()][temp.getX()] = ROAD;
			}
		}
		else if (temp.isOnRandom())
		{
			// random event is triggered
			random.randEvent(*this);
			// need to delete the icon, check the coordinate
			gMap[temp.getY()][temp.getX()] = ROAD;
		}
	}
}

bool Role::flee()
{
	system("cls");
	cout << "|| ¡´ You have to roll ONE dice to flee.\n|| ¡´ But, the maximum possibility is 98%.\n";
	double successRate = vitality / (100 + pAttack + mAttack);
	if (successRate > 0.98) { successRate = 0.98; }
	cout << endl << "|| ¡´ Success Rate is: " << successRate << endl;
	cout << "|| ¡´ Press any key to roll the dice!\n";
	char input = _getch();
	rollingAnime();

	if ((double)(rand() % 100) / 100 < successRate) {
		return true;
	}
	return false;
}

void Role::attack(int enemyNum, vector<Enemy*>& es, int pHarm, int eAbsorption, int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is attack, you need to roll ONE dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use ONE focus? (Y/N):";
	char input = _getch();
	cout << input << endl;

	if (input == 'Y') { // use focus, so it can attack
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << "T";
		colorSettings = BACKGROUND_INTENSITY;
		colorSettings = FOREGROUND_INTENSITY;
		SetConsoleTextAttribute(hConsole, colorSettings);

		cout << "\n|| ¡´ You can attack the enemy!\n";

		// caculate harm and absorption
		eAbsorption = es[enemyNum]->getMDefense() / (es[enemyNum]->getMDefense() + 50);
		pHarm = floor(pAttack * (1 - eAbsorption));
		cout << "|| ¡´ You caused " << pHarm << " damage on " << enemyNum << "'s enemy!\n";
		es[enemyNum]->setVitality(es[enemyNum]->getVitality() - pHarm);
	}
	else { // no focus, roll dice
		rollingAnime();

		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // if angryDebuff is not 0
		}
		cout << "|| ¡´ Success Rate is: " << successRate << endl;
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		// roll dice
		if (rand() % 100 < successRate) { // success
			cout << "T\n";
			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);

			cout << "|| ¡´ You can attack the enemy!\n";

			// caculate harm and absorption
			eAbsorption = es[enemyNum]->getMDefense() / (es[enemyNum]->getMDefense() + 50);
			pHarm = floor(pAttack * (1 - eAbsorption));
			cout << "|| ¡´ You caused " << pHarm << " damage on " << enemyNum << "'s enemy!\n";
			es[enemyNum]->setVitality(es[enemyNum]->getVitality() - pHarm);
		}
		else { // failed
			cout << "F";
			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "\n|| ¡´ So sad, you failed to attack this enemy:(\n";
		}
	}
}

void Role::provoke(int enemyNum, vector<Enemy*>& es, int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is provoke, you need to roll ONE dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use ONE focus? (Y/N):";
	char input = _getch();
	cout << input << endl;

	if (input == 'Y') { // use focus, so it can attack
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << "T\n";
		colorSettings = BACKGROUND_INTENSITY;
		colorSettings = FOREGROUND_INTENSITY;
		SetConsoleTextAttribute(hConsole, colorSettings);

		cout << "|| ¡´ You can use provoke!\n" <<
			"|| ¡´ Now this enemy has angry debuff!\n";
		es[enemyNum]->angryDebuff += 3;
	}
	else {
		rollingAnime();

		successRate = vitality / (100 + pAttack + mAttack);
		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // if angryDebuff is not 0
		}
		cout << "|| ¡´ Success Rate is: " << successRate << endl;
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		// roll dice
		if (rand() % 100 < successRate) { // success
			cout << "T\n";
			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);

			cout << "|| ¡´ You can use provoke!\n" <<
				"|| ¡´ Now this enemy has angry debuff!\n";
			es[enemyNum]->angryDebuff += 3;
		}
		else { // failed
			cout << "F\n";
			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "|| ¡´ So sad, you failed to provoke this enemy:(\n";
		}
	}
}

void Role::shockBlast(vector<Enemy*>& es, int mHarm, int eAbsorption, int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is shockBlast, you need to roll 3 dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use focus? (Y/N):";
	char input = _getch();
	cout << input << endl;

	int successDice = 0;

	if (input == 'Y') {
		cout << "|| ¡´ How many focus you're going to use: ";
		input = _getch();
		while (input - '0' > focus) {
			cout << input;
			cout << endl << "|| ¡´ invalid amount, enter again: ";
		}
		cout << input << "|| ¡´ Press any key to roll the dice!\n";
		input = _getch();
		rollingAnime();

		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		cout << "|| ¡´ Result: ";

		int a = input - '0';

		for (int i = 0; i < a; i++) { 
			cout << 'T';
			successDice++;
		}
		for (int i = 0; i < 3 - a; i++) { 
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
	}
	else { // no focus
		cout << "|| ¡´ Press any key to roll the dice!\n";
		input = _getch();
		rollingAnime();

		successRate -= 5;
		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // if angryDebuff is not 0
		}
		cout << "|| ¡´ Success Rate is: " << successRate << endl;
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		int successDice = 0;

		for (int i = 0; i < 3; i++) { // roll three dice
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
	}

	// testing ac rate
	cout << endl << "ac rate: " << successDice << endl;

	// after rolling dice
	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	if (successDice) { // there's at least one T dice
		mHarm = floor(mAttack * 1.5 * (successDice / 3));
		cout << "|| ¡´ You can use shockBlast!\n|| ¡´ You can caused attack every enemy!\n";
		for (int i = 0; i < es.size(); i++) { // cause damage on every enemy
			if (!es[i]->getIsDead()) { // check vitality of every enemy
				eAbsorption = es[i]->getMDefense() / (es[i]->getMDefense() + 50);
				mHarm = floor(mAttack * (1 - eAbsorption));
				cout << "|| ¡´ You caused " << mHarm << " damage on " << i << "'s enemy!\n";
				es[i]->setVitality(es[i]->getVitality() - mHarm);
			}
		}
	}
	else { // failed
		cout << "|| ¡´ So sad, you failed to use shockBlast:(\n";
	}
}

void Role::heal(int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is heal, you need to roll TWO dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use focus? (Y/N):";
	char input = _getch();
	cout << input << endl;

	int successDice = 0;

	if (input == 'Y') {
		cout << "|| ¡´ How many focus you're going to use: ";
		input = _getch();
		while (input - '0' > focus) {
			cout << input;
			cout << endl << "|| ¡´ invalid amount, enter again: ";
		}
		cout << input << "|| ¡´ Press any key to roll the dice!\n";
		input = _getch();
		rollingAnime();

		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		cout << "|| ¡´ Result: ";

		int a = input - '0';

		for (int i = 0; i < a; i++) {
			cout << 'T';
			successDice++;
		}
		for (int i = 0; i < 3 - a; i++) {
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
	}
	else { // no focus
		rollingAnime();

		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // if angryDebuff is not 0
		}

		cout << "|| ¡´ Success Rate is: " << successRate << endl;
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		int successDice = 0;

		for (int i = 0; i < 2; i++) { // roll three dice
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
		cout << endl;
	}
	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	double a = (double)successDice / 2.0;

	if (!successDice) { // there's at least one T dice
		cout << "|| ¡´ You can use heal!\n";
		cout << "|| ¡´ You can regain " << floor(mAttack * 1.5 * a) << vitality;
		vitality += floor(mAttack * 1.5 * a);
		if (vitality > 100) { vitality = 100; } // upper limit is 100
	}
	else { // failed
		cout << "\n|| ¡´ So sad, you failed to use heal:(\n";
	}
}

void Role::speedUp(int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is speedUp, you need to roll TWO dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use focus? (Y/N):";
	char input = _getch();
	cout << input << endl;

	int successDice = 0;

	if (input == 'Y') {
		cout << "|| ¡´ How many focus you're going to use: ";
		input = _getch();
		while (input - '0' > focus) {
			cout << input;
			cout << endl << "|| ¡´ invalid amount, enter again: ";
		}
		cout << input << "|| ¡´ Press any key to roll the dice!\n";
		input = _getch();
		rollingAnime();

		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		cout << "|| ¡´ Result: ";

		int a = input - '0';

		for (int i = 0; i < a; i++) {
			cout << 'T';
			successDice++;
		}
		for (int i = 0; i < 3 - a; i++) {
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
	}
	else {
		rollingAnime();

		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // if angryDebuff is not 0
		}

		cout << "|| ¡´ Success Rate is: " << successRate << endl;
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		int successDice = 0;

		for (int i = 0; i < 2; i++) { // roll three dice
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
		cout << endl;
	}

	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	if (!successDice) { // there's at least one T dice
		cout << "|| ¡´ You can use speedUp!\n";
		speedUpCool = 1;
		speed *= 1.5;
		cout << "|| ¡´ Your speed now is " << speed << "!\n";
	}
	else { // failed
		cout << "\n|| ¡´ So sad, you failed to use speedUp:(\n";
	}
}

// passive skill
void Role::hammerSplash(vector<Enemy*>& es, int pHarm, int eAbsorption)
{
	cout << "|| ¡´ You activated the PASSIVE SKILL hammer-splash!\n"
		<< "|| ¡´ You can cause extra 50% damage on each enemy!\n";
	char input = _getch();

	// caculate harm and absorption
	for (int i = 0; i < es.size(); i++) {
		if (!es[i]->getIsDead()) { // this enemy is not dead yet
			eAbsorption = es[i]->getMDefense() / (es[i]->getMDefense() + 50);
			pHarm = floor(pAttack * 0.5 * (1 - eAbsorption));
			cout << "|| ¡´ You caused " << pHarm << " damage on " << i << "'s enemy!\n";
			es[i]->setVitality(es[i]->getVitality() - pHarm);
		}
	}
}

bool isCombatOver(Role& player, vector<Enemy*> es)
{
	// if player use flee, inBattle will be false
	if (!player.getInBattle()) { return true; }
	for (int i = 0; i < 3; i++)
	{
		// even just one alive, then combat continue
		if (!es[i]->getIsDead()) { return false; }
	}
	return true;
}

// show all enemies and player's status
void Role::showAllStatus(vector<Enemy*>& es)
{
	// print out icon and status
	for (int i = 0; i < 3; i++) {
		cout << 'E' << i + 1 << "'s status:\n";
		if (es[i]->getIsDead()) {
			cout << "|| ¡´ This enemy is dead.\n";
		}
		else {
			es[i]->showStatus();
		}
		cout << endl;
	}

	showStatus();
}

// this function return a bool which represent if the user has chosen a valid skill
// also, it updates variable curSkill, curSkill is the chosen skill which is going to be used in battle phase
bool Role::chooseSkill()
{
	system("cls");
	cout << "||=====================================||\n";
	cout << "||===== It's your turn to attack! =====||\n";
	cout << "||=====================================||\n\n";
	showStatus();
	cout << "\n||===== Here's avalible equipment =====||\n";
	for (auto& pair : skillEquipMap) {
		if (avalEquip[pair.second] && pair.second != "other") { // if the equip exists
			// equip -> skill
			cout << pair.second << " -> " << pair.first << endl;
		}
	}

	cout << "\n\n|| ¡´ ENTER TO CHOOSE YOUR SKILL ¡´ ||\n";

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings |= BACKGROUND_INTENSITY;

	// print out current chosen skill
	for (int i = 0; i < skillVec.size(); i++) {
		// the chosen one will be red
		if (i == numSkill) { colorSettings = FOREGROUND_RED; }
		else { colorSettings = FOREGROUND_INTENSITY; }
		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << skillVec[i] << std::endl;
	}

	// for the last output
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	char input = _getch();

	if (input == 72 && numSkill > 0) { // up is clicked
		numSkill--;
	}
	else if (input == 80 && numSkill < skillVec.size() - 1) { // down is clicked
		numSkill++;
	}
	else if (input == 13) { // enter is clicked
		// check if the required equipment of this skill is valiable
		if (avalEquip[skillEquipMap[skillVec[numSkill]]]) {
			curSkill = skillVec[numSkill];
			return true;
		}
		else { // player doesn't have this equipment, so that skill is invalid
			cout << "You don't have the cooresponding equipment to use this skill!\n";
			cout << "/** Press any key to continue **/\n";
			input = _getch();
		}
	}

	// to keep while loop running
	return false;
}

void Role::chooseItem()
{
	system("cls");
	cout << "||=====================================||\n";
	cout << "||===== It's your turn to attack! =====||\n";
	cout << "||=====================================||\n\n\n";

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;

	// hightlight the skill
	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED;
	SetConsoleTextAttribute(hConsole, colorSettings);
	cout << "|| ¡´ You've chosen " << skillVec[numSkill] << " ¡´ || \n\n";

	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	char input;

	if (godsBeard == 0 && goldenRoot == 0) { // no item avaliable
		cout << "You don't have any item to use. Click eny key to exit.\n";
	}
	else {
		cout << "Now you can choose if you want to apply special item.\n1. godsBeard adds 25 points vitality.\n2. goldenRoot adds 3 focus.\n\n";
		// check items seperately
		if (godsBeard) {
			cout << "|| ¡´ You have " << godsBeard << " godsBeard.\n";
			cout << "|| ¡´ Click ENTER to apply ONE ";
			colorSettings = FOREGROUND_RED;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "godsBeard.";
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "\n|| ¡´ Or click any key otherwise.\n";

			input = _getch();
			if (input == '\r') { // use godsBeard
				if (vitality > 75) { vitality = 100; } // vitality upper limit is 100
				else { vitality += 25; }
				godsBeard--;
				// hightlight the vitality
				colorSettings = BACKGROUND_GREEN;
				colorSettings |= BACKGROUND_RED;
				SetConsoleTextAttribute(hConsole, colorSettings);
				cout << "|| ¡´ You've used godsBeard! ¡´ || \n";
				cout << "|| ¡´ Your vitality is : " << vitality << " ¡´ || \n";
				colorSettings = BACKGROUND_INTENSITY;
				colorSettings = FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(hConsole, colorSettings);
			}
		}
			
		if (goldenRoot) {
			cout << "|| ¡´ You have " << goldenRoot << " godsBeard.\n";
			cout << "|| ¡´ Click ENTER to apply ONE ";
			colorSettings = FOREGROUND_RED;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "goldenRoot.";
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "\n|| ¡´ Or click any key otherwise.\n";

			input = _getch();
			if (input == '\r') {
				focus += 3;
				goldenRoot--;
				// hightlight the focus
				colorSettings = BACKGROUND_GREEN;
				colorSettings |= BACKGROUND_RED;
				SetConsoleTextAttribute(hConsole, colorSettings);
				cout << "|| ¡´ You've used goldenRoot! ¡´ || \n";
				cout << "|| ¡´ Your focus is : " << focus << endl;
				colorSettings = BACKGROUND_INTENSITY;
				colorSettings = FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(hConsole, colorSettings);
			}
		}
	}
	cout << "/** Click any key to continue... **/";
	input = _getch();
}

int Role::chooseEnemy(vector<Enemy*>& es)
{
	system("cls");
	cout << "||=====================================||\n";
	cout << "||===== It's your turn to attack! =====||\n";
	cout << "||=====================================||\n\n\n";
	cout << "|| ¡´ Now, you need to choose ONE enemy to attak.\n"
		"|| ¡´ The following shows all status of each alive enemy.\n\n";

	for (int i = 0; i < es.size(); i++) {
		if (!es[i]->getIsDead()) { // if this enemy is not dead
			cout << "the " << i + 1 << "'s enemy: \n";
			es[i]->showStatus();
			cout << endl << endl;
		}
	}

	cout << "Enter the number of enemy that you want to attack: ";
	char num = _getch();
	int a = num - '0' - 1;
	while (es[a]->getIsDead()) { // the chosen enemy should not be dead
		cout << num;
		cout << "\nThis enemy is dead, choose again:";
		num = _getch();
		a = num - '0' - 1;
	}
	cout << num << endl;

	cout << "\n/** Click any key to continue... **/";
	num = _getch();

	// return the index of the enemy that has been chosen
	return a;
}

void Role::attackEnemy(vector<Enemy*> &es, int enemyNum, int angryDebuff)
{
	system("cls");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	cout << "||=====================================||\n";
	cout << "||===== It's your turn to attack! =====||\n";
	cout << "||=====================================||\n\n\n";
	
	char input;
	int successRate = hitRate;

	// assign back to the original after this round of attacking
	int tempPA = pAttack, tempPD = pDefense, tempMA = mAttack, tempMD = mDefense, tempS = speed;
	cout << "|| ¡´ The following are your activated equipment and attack power: \n\n";

	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED;
	SetConsoleTextAttribute(hConsole, colorSettings);

	// check every equipment and apply it
	for (auto& pair : equipFuncList) {
		if (avalEquip[pair.first]) {
			cout << pair.first << " ";

			// player can activate multiple equipment at once, use for loop
			for (int i = 0; i < avalEquipNum[pair.first]; i++) { 
				equipFuncList[pair.first];
			}

			// print out the amount of equip
			cout << " -> " << avalEquipNum[pair.first] << "X\n";
		}
	}
	cout << endl << endl;
	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);
	showStatus();

	cout << "\n/** Press any key to continue... **/\n";
	input = _getch();

	int eAbsorption = 0, pHarm = 0, mHarm = 0;

	if (curSkill == "attack") {
		attack(enemyNum, es, pHarm, eAbsorption, successRate, hConsole, colorSettings);
		if (avalEquip["giantManner"]) {
			hammerSplash(es, pHarm, eAbsorption);
		}
	}
	else if (curSkill == "provoke") {
		provoke(enemyNum, es, successRate, hConsole, colorSettings);
	}
	else if (curSkill == "shockBlast") {
		shockBlast(es, mHarm, eAbsorption, successRate, hConsole, colorSettings);
	}
	else if (curSkill == "heal") {
		heal(successRate, hConsole, colorSettings);
	}
	else if (curSkill == "speedUp") {
		speedUp(successRate, hConsole, colorSettings);
	}

	// check if this enemy is dead or not, if not,  print out the status of this enemy
	if (es[enemyNum]->getVitality() <= 0) { // dead
		cout << "|| ¡´ This enemy is dead! You earn 700 coins and a godsBeard as a prize!\n";
		money += 700;
		godsBeard++;
		es[enemyNum]->setVitality(0); // set to 0
		es[enemyNum]->setDead(true);
	}
	else {
		cout << "\n|| ¡´ The following is the new status of this enemy:\n";
		es[enemyNum]->showStatus();
	}

	if (!angryDebuff) { angryDebuff--; }

	// Since items activate every time before attacking, 
	// all status must return to original status after every round,
	pAttack = tempPA;
	pDefense = tempPD;
	mAttack = tempMA;
	mDefense = tempMD;
	speed = tempS;

	cout << "\n/** Your turn is over. Click any key to the combat field.\n";
	input = _getch();
}

bool Role::combat()
{
	// set up enemies
	vector<Enemy*> es;
	for (int i = 0; i < 3; i++) {
		Enemy* enem = new Enemy();
		es.push_back(enem);
	}

	// in battle phase
	inBattle = true;
	char input;

	// combat loop
	while (!isCombatOver(*this, es))
	{
		system("cls");

		// this is only for not in battle field (before take turn to attack)
		showAllStatus(es);
		cout << "/** Click any key to continue **/\n";
		input = _getch();
		
		/*
		* First! get the priority of every entity!
		*/


		// order array will help to set the order, for instance, 
		/* { 3 1 2 0 } means player goes first, then the enemy of index of 1, 2, 0
		// 0-2 are enymies' indexes of vetor, 3 is player
		*/ 
		int order[4] = { 0,1,2,3 };
		int pri = 0;

		// {index, pri}
		pair<int, int> priority[4] = { {0, 0}, {1, 0}, {2, 0}, {3, 0} };
		pair<int, int> newPriority[4] = { {0, 0}, {1, 0}, {2, 0}, {3, 0} };

		// decide order
		for (int i = 0; i < 4; i++) {

			// priority[i].first is index
			if (priority[i].first != 3) {
				pri = (es[priority[i].first]->round + 1) / es[priority[i].first]->getSpeed();
				newPriority[i] = make_pair(priority[i].first, pri);
			}
			else {
				pri = (round + 1) / getSpeed();
				newPriority[i] = make_pair(3, pri);
			}
		}
		copy(newPriority, newPriority + 4, priority);
		
		// sort priority by the first 
		std::sort(priority, priority + 4, [](const auto& a, const auto& b) {
			return a.second < b.second;
		});

		// test order
		cout << endl << "the order will be: \n";
		for (auto& pair : priority) {
			cout << pair.first << ' ';
		}
		cout << endl;

		// testing cout
		cout << "click to continue";
		input = _getch();

		/*
		* Second! Start to attack!
		*/
		// initialize all harm and debuff setting
		int pHarm = 0, mHarm = 0, absorption = 0, enemyNum = -1;
		bool UseAngryDebuff = false;

		// battle phase, take turn
		for (auto& pair : priority) {
			
			// for printing out only one entity on the console
			system("cls");

			// enemy's turn to attak player
			if (pair.first != 3 && !es[pair.first]->getIsDead()) { 
				cout << "||=====================================||\n";
				cout << "||=== It's E" << pair.first + 1 << "'s turn to roll dice! ====||\n";
				cout << "||=====================================||\n\n\n";

				// caculate harm on player
				switch (rand() % 5) {
				case 0:
					pHarm = es[pair.first]->attack(es[pair.first]->getHitRate());
					cout << "|| ¡¶ It used PHYSICAL ATTACK!! \n";
					if (pHarm == 0) {
						cout << "|| ¡¶ You are lucky! \n"
							"|| ¡¶ This enemy failed to have any valid dice!\n"
							<< "|| ¡¶ You didn't get the angry debuff!\n";
					}
					else {
						if (avalEquip["PlateArmor"]) {
							cout << "|| ¡´ Since you've activated the PlateArmor,\n|| ¡´ You have the passive skill FORTIFY, which reduce 10% damage!\n";
							
							// for testing
							cout << "original pHarm: " << pHarm << endl;
							
							pHarm *= 0.9;
						}
						cout << "|| ¡¶ DAMAGE: " << pHarm << "\n\n";
					}
					
					break;
				case 1:
					UseAngryDebuff = es[pair.first]->provoke(es[pair.first]->getHitRate());
					cout << "|| ¡¶ It used PROVOKE!! \n";
					if (!UseAngryDebuff) {
						cout << "|| ¡¶ You are lucky! \n"
							"|| ¡¶ This enemy failed to have any valid dice!\n"
							<< "|| ¡¶ You didn't get the angrt debuff!\n";
					}
					break;
				case 2:
					mHarm = es[pair.first]->shockBlast(es[pair.first]->getHitRate());
					cout << "|| ¡¶ It used MAGIC ATTACK!! \n";
					if (avalEquip["PlateArmor"]) {
						cout << "|| ¡´ Since you've activated the PlateArmor,\n|| ¡´ You have the passive skill FORTIFY, which reduce 10% damage!\n";

						// for testing
						cout << "original mHarm: " << mHarm << endl;
						
						mHarm *= 0.9;
					}
					cout << "|| ¡¶ DAMAGE: " << mHarm << "\n";
					break;
				case 3:
					mHarm = es[pair.first]->heal(es[pair.first]->getHitRate());  // attack 
					cout << "|| ¡¶ It used HEAL!! \n";;
					if (mHarm != 0) { 
						vitality = 100; 
						cout << "|| ¡¶ This enemy re-fill the vitality th 100!!\n";
					}  // self heal
					else {
						cout << "|| ¡¶ But it didn't re-fill its vitality.\n";
					}
					break;

				/** buff not defined yet!!! 
				case 4:
					harm = es[pair.first]->speedUp(); break;
				**/

				default:
					cout << "|| ¡¶ You are lucky! \n"
						"|| ¡¶ This enemy failed to have any valid dice!\n";
				}

				es[pair.first]->round++;
				
				// update player status
				if (UseAngryDebuff) {
					UseAngryDebuff = false;
					angryDebuff += 3;
					cout << "|| ¡¶ You've gotten ANGRY DEBUFF!!\n";
					cout << "|| ¡´ You have angry debuff: " << angryDebuff << endl;
				}
				else {
					// check if it's mAttack or pAttack
					if (mHarm != 0) {
						absorption = (double)mAttack / (mAttack + 50);
						mHarm *= (1 - absorption);
						vitality -= mHarm;
						cout << "|| ¡´ Your vitality: " << vitality << endl;
					}
					else if (pHarm != 0) {
						absorption = (double)pAttack / (pAttack + 50);
						pHarm *= (1 - absorption);
						vitality -= pHarm;
						cout << "|| ¡´ Your vitality: " << vitality << endl;
					}
				}

				// initilize the damage after enemy's attack
				pHarm = 0;
				mHarm = 0;

				cout << "\n/** Press Enter to continue... **/\n";
				input = _getch();
				while (input != '\r') {
					input = _getch();
				}

			}
			else { // player's turn to throw attack
				
				// for showing the chosen skill, and remember the skill name
				numSkill = 0;

				// run until the chosen skill is valid
				while (1) {
					if (chooseSkill()) { break; }
				}

				// check flee
				if (curSkill == "flee") {
					if (flee()) {
						cout << "You've flee successfully, click any key to exit.";
						input = _getch();
						inBattle = false;
						return true;
					}
					cout << "You filed to roll a valid dice. Press any key to keep fighting!\n";
					input = _getch();
				}
				else {
					// choose item before combat
					chooseItem();

					if (curSkill != "heal" && curSkill != "speedUp") {
						// choose enemy to attack
						enemyNum = chooseEnemy(es);
						cout << enemyNum << endl;
						Sleep(700);
					}
				}
				attackEnemy(es, enemyNum, angryDebuff);
				
				// check angry debuff before throwing dice
			}
		}
	}

	// initialize all buff/debuff
	angryDebuff = 0;
	speedUpCool = 0;
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
	cout << "||============= Your status =============||\n\n";
	cout << "|| ¡´ Vitality: " << setw(10) << vitality << '\n'
		<< "|| ¡´ focus: " << setw(13) << focus << '\n'
		<< "|| ¡´ speed: " << setw(13) << speed << '\n'
		<< "|| ¡´ hitRate: " << setw(11) << hitRate << '\n'
		<< "|| ¡´ physical Attack: " << setw(3) << pAttack << '\n'
		<< "|| ¡´ magic Attack: " << setw(6) << mAttack << '\n'
		<< "|| ¡´ physical Defense: " << setw(2) << pDefense << '\n'
		<< "|| ¡´ magic Defense: " << setw(5) << mDefense << '\n'
		<< "|| ¡´ godSbeard: " << setw(9) << godsBeard << '\n'
		<< "|| ¡´ goldenRoot: " << setw(8) << goldenRoot << '\n'
		<< "|| ¡´ Money: " << setw(13) << money << '\n';
}

// for taking steps
void Role::throwDice()
{
	cout << "||=====================================||\n";
	cout << "||=== It's your turn to roll dice! ====||\n";
	cout << "||=====================================||\n\n\n";
	cout << "|| ¡´ You have " << getFocus() << " focus left.\n";
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

		while (input != '\r')
		{
			input = _getch();
		}
		showRollDice(0);
	}
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
	cout << "\n\nYou have " << avalstep << " True in this round.\n";

	cout << "\n/** Click any key to exit. **/\n";
	chance = _getch();
}

