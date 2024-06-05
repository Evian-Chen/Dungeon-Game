/***********************************************************************
 * File: Role.cpp
 * Description: Implementation of the Role class which handles player interactions,
				movements, combat, and inventory management in the game. The Role class inherits
				from the Entity class and includes methods for dice rolling animations, combat,
				and interactions with the game's environment and entities.
 ***********************************************************************/

#include "Role.h"
#include "Enemy.h"
#include "RandomEvent.h"
#include "Position.h"
#include "allConstants.h"
#include "mapElement.h"

#include <utility>
#include <algorithm>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

// Static variables initialization
int Role::money = 600;
int Role::diceLimit = 1;
int Role::godsBeard = 3;
int Role::goldenRoot = 3;

// Global instance for random events
RandomEvent random;
// Global list of enemies
vector<Enemy*> enemy;

// Function to animate dice rolling
void rollingAnime()
{
	cout << "\n\n||=== generating the result... ====||\n";
	cout << "|| ";

	// Handle for console settings
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// Initial color settings for the console
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED;
	SetConsoleTextAttribute(hConsole, colorSettings);

	// Animation loop
	for (int i = 0; i < 31; i++)
	{
		cout << ' ';
		Sleep(100); // Delay to mimic rolling
	}

	// Reset color settings
	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	cout << " ||\n\n";
}

// Function to display winning animation
void youWinAnime() {
	std::string youWinArt = R"(
	__     ______  _    _   _    _  __        _____ _   _ 
	\ \   / / __ \| |  | | | |  | | \ \      / /_ _| \ | |
	 \ \_/ / |  | | |  | | | |  | |  \ \ /\ / / | ||  \| |
	  \   /| |  | | |  | | | |  | |   \ V  V /  | || . ` |
	   | | | |__| | |__| | | |__| |    \_/\_/  | || |\  |
	   |_|  \____/ \____/   \____/              |_|_| \_|

	)";
	std::cout << youWinArt << std::endl;
}

// Move method for Role class, handling player movement and interactions
bool Role::move(const Position& delta) {
	Position temp = position + delta;

	// Handle different scenarios based on player's movement and location
	if (avalstep == 0)
	{
		if (diceLimit == 0) {
			setDead(true);
		}
		else {
			// Player needs to throw dice if no available steps
			throwDice();
			diceLimit--;
		}
	}
	else
	{
		// Check if new position is valid
		if (temp.isPositionValid())
		{
			position += delta;
			avalstep--;
		}
		// Interact with a shop
		else if (temp.isOnShop())
		{
			cout << "======== Do you want to go to the shop? (Y/N) ========\nY/N:";
			char input = _getch();
			while (input != 'Y' && input != 'N') {
				cout << input << endl;
				cout << "Invalid input, enter again: ";
				input = _getch();
			}
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
		// Interact with hot springs
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
					if (vitality >= 70) { vitality = 100; }
					else { vitality += 30; }
				}
			}
			else if (input == 'N')
			{
				cout << input << "\nOkay, then you can click any key to get out. >:(\n";
				input = _getch();
			}
			gMap[temp.getY()][temp.getX()] = ROAD;
		}
		// Interaction with an enemy
		else if (temp.isOnEnemy()) {
			if (combat()) {} // if player flee, then combat returns true
			else {
				// If combat ends normally without fleeing, clear the enemy from the map
				gMap[temp.getY()][temp.getX()] = ROAD;
			}
		}
		// Interaction with a random event on the map
		else if (temp.isOnRandom()) { // random event is triggered
			random.randEvent(*this);
			// need to delete the icon, check the coordinate
			gMap[temp.getY()][temp.getX()] = ROAD;
		}
		// Interaction with the final destination or objective
		else if (temp.isOnFinal()) {
			system("cls");
			youWinAnime(); // winning anmation
			return false;  // Return false to indicate the game or level completion
		}
	}
	return true;
}

// Method to simulate the player attempting to flee from combat
bool Role::flee()
{
	system("cls");
	cout << "|| ¡´ You have to roll ONE dice to flee.\n|| ¡´ But, the maximum possibility is 98%.\n";
	double successRate = (double)vitality / (100 + pAttack + mAttack);
	if (successRate > 0.98) { successRate = 0.98; }
	cout << endl << "|| ¡´ Success Rate is: " << successRate << endl;
	cout << "|| ¡´ Press any key to roll the dice!\n";
	char input = _getch();
	rollingAnime();

	if ((double)(rand() % 100) / 100 < successRate) {
		setInBattle(false); // Set inBattle to false, indicating player has successfully fled
		return true; // Return true to indicate successful flee
	}
	return false; // Return false to continue the combat
}

// Method to handle attacking logic in combat
void Role::attack(int enemyNum, vector<Enemy*>& es, int pHarm, int eAbsorption, int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is attack, you need to roll ONE dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use ONE focus? (Y/N):";
	char input = _getch();

	while (input != 'Y' && input != 'N') {
		cout << input << endl;
		cout << "\nInvalid input, enter again: ";
		input = _getch();
	}
	cout << input << endl;

	if (input == 'Y') { // If player decides to use focus
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << "T";
		colorSettings = BACKGROUND_INTENSITY;
		colorSettings = FOREGROUND_INTENSITY;
		SetConsoleTextAttribute(hConsole, colorSettings);

		setFocus(getFocus() - 1); // Decrease focus by one

		cout << "\n|| ¡´ You can attack the enemy!\n";

		// Calculate harm based on magic defense of the enemy
		eAbsorption = es[enemyNum]->getMDefense() / (es[enemyNum]->getMDefense() + 50);
		pHarm = floor(pAttack * (1 - eAbsorption));
		cout << "|| ¡´ You caused " << pHarm << " damage on " << enemyNum + 1 << "'s enemy!\n";
		es[enemyNum]->setVitality(es[enemyNum]->getVitality() - pHarm);
	}
	else { // If no focus is used, player must roll dice
		rollingAnime();

		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // Modify success rate based on angryDebuff
		}
		cout << "|| ¡´ Success Rate is: " << successRate << endl;
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		// Roll dice and check if the attack is successful
		if (rand() % 100 < successRate) { // If attack succeeds
			cout << "T\n";
			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);

			cout << "|| ¡´ You can attack the enemy!\n";

			// Calculate harm and update enemy's vitality
			eAbsorption = es[enemyNum]->getMDefense() / (es[enemyNum]->getMDefense() + 50);
			pHarm = floor(pAttack * (1 - eAbsorption));
			cout << "|| ¡´ You caused " << pHarm << " damage on " << enemyNum + 1 << "'s enemy!\n";
			es[enemyNum]->setVitality(es[enemyNum]->getVitality() - pHarm);
		}
		else { // If attack fails
			cout << "F";
			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "\n|| ¡´ So sad, you failed to attack this enemy:(\n";
		}
	}
}

// Provoke an enemy to increase its aggression but potentially increase the risk to the player
void Role::provoke(int enemyNum, vector<Enemy*>& es, int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls"); // Clear console for clean output
	cout << "|| ¡´ Since your skill is provoke, you need to roll ONE dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use ONE focus? (Y/N):";
	char input = _getch();

	while (input != 'Y' && input != 'N') {
		cout << input << endl;
		cout << "\nInvalid input, enter again: ";
		input = _getch();
	}
	cout << input << endl;

	if (input == 'Y') { // If user chooses to use focus
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN; // Set console color for success
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << "T\n"; // Display success indicator
		colorSettings = BACKGROUND_INTENSITY;
		colorSettings = FOREGROUND_INTENSITY;
		SetConsoleTextAttribute(hConsole, colorSettings);

		setFocus(getFocus() - 1); // Reduce focus

		cout << "|| ¡´ You can use provoke!\n" <<
			"|| ¡´ Now this enemy has angry debuff!\n";
		es[enemyNum]->angryDebuff += 3; // Apply debuff to enemy
	}
	else {
		rollingAnime(); // Show animation for dice roll

		successRate = vitality / (100 + pAttack + mAttack); // Calculate new success rate
		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // Adjust success rate based on angryDebuff
		}
		cout << "|| ¡´ Success Rate is: " << successRate << endl;
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		// Roll dice to determine outcome
		if (rand() % 100 < successRate) { // If success
			cout << "T\n";
			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);

			cout << "|| ¡´ You can use provoke!\n" <<
				"|| ¡´ Now this enemy has angry debuff!\n";
			es[enemyNum]->angryDebuff += 3;
		}
		else { // If failed
			cout << "F\n";
			colorSettings = BACKGROUND_INTENSITY;
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "|| ¡´ So sad, you failed to provoke this enemy:(\n";
		}
	}
}

// ShockBlast targets multiple enemies with a magic attack that requires rolling three dice
void Role::shockBlast(vector<Enemy*>& es, int mHarm, int eAbsorption, int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is shockBlast, you need to roll 3 dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use focus? (Y/N):";
	char input = _getch();

	while (input != 'Y' && input != 'N') {
		cout << input << endl;
		cout << "\nInvalid input, enter again: ";
		input = _getch();
	}
	cout << input << endl;

	int successDice = 0;

	if (input == 'Y') { // If focus is used
		cout << "|| ¡´ How many focus you're going to use (maximun 9): ";
		input = _getch();
		while (input - '0' > focus) {
			cout << input;
			cout << endl << "|| ¡´ invalid amount, enter again: ";
		}
		int num = input - '0';
		setFocus(getFocus() - num); // Reduce focus by number chosen
		cout << input << "\n|| ¡´ Press any key to roll the dice!\n";
		input = _getch();
		rollingAnime();

		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		cout << "|| ¡´ Result: ";

		for (int i = 0; i < num; i++) {
			cout << 'T';
			successDice++;
		}
		for (int i = 0; i < 3 - num; i++) {
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
	}
	else { // If no focus is used
		cout << "|| ¡´ Press any key to roll the dice!\n";
		input = _getch();
		rollingAnime();

		successRate -= 5; // Slightly reduce success rate
		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // Adjust for angryDebuff
		}
		cout << "|| ¡´ Success Rate is: " << successRate << endl;
		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		int successDice = 0;

		for (int i = 0; i < 3; i++) { // Roll three dice
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
	}

	// Evaluate the result after dice roll
	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	if (successDice) { // If successful
		mHarm = floor(mAttack * 1.5 * (successDice / 3)); // Calculate magic harm
		cout << "|| ¡´ You can use shockBlast!\n|| ¡´ You can caused attack every enemy!\n";
		for (int i = 0; i < es.size(); i++) { // Apply damage to every enemy
			if (!es[i]->getIsDead()) { // Check if enemy is alive
				eAbsorption = es[i]->getMDefense() / (es[i]->getMDefense() + 50);
				mHarm = floor(mAttack * (1 - eAbsorption));
				cout << "|| ¡´ You caused " << mHarm << " damage on " << i + 1 << "'s enemy!\n";
				es[i]->setVitality(es[i]->getVitality() - mHarm);
			}
		}
	}
	else { // If failed
		cout << "|| ¡´ So sad, you failed to use shockBlast:(\n";
	}
}

// Heal allows the player to regain vitality by rolling two dice to determine success
void Role::heal(int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is heal, you need to roll TWO dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use focus? (Y/N):";
	char input = _getch();

	while (input != 'Y' && input != 'N') {
		cout << input << endl;
		cout << "\nInvalid input, enter again: ";
		input = _getch();
	}
	cout << input << endl;

	int successDice = 0;

	if (input == 'Y') {
		cout << "how many focus you want to use: ";
		input = _getch();
		while (input - '0' > focus) {
			cout << input;
			cout << endl << "\n|| ¡´ invalid amount, enter again: ";
		}
		int num = input - '0';
		setFocus(getFocus() - num); // Reduce focus by chosen number
		cout << input << "\n|| ¡´ Press any key to roll the dice!\n";
		input = _getch();
		rollingAnime();

		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		cout << "|| ¡´ Result: ";

		for (int i = 0; i < num; i++) {
			cout << 'T';
			successDice++;
		}
		for (int i = 0; i < 2 - num; i++) {
			if (rand() % 100 < successRate) {
				cout << "T";
				successDice++;
			}
			else { cout << "F"; }
		}
	}
	else { // If no focus is used
		rollingAnime();

		if (!angryDebuff) {
			if (successRate >= 30) { successRate -= 30; } // Adjust for angryDebuff
		}

		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		for (int i = 0; i < 2; i++) { // Roll two dice
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

	if (successDice) { // If successful
		cout << "\n|| ¡´ You can use heal!\n";
		cout << "|| ¡´ You can regain " << floor(mAttack * 1.5 * a) << " vitality.\n";
		setVitality(floor(mAttack * 1.5 * a));
		if (getVitality() > 100) { setVitality(100); } // Ensure vitality does not exceed maximum
	}
	else { // If failed
		cout << "\n|| ¡´ So sad, you failed to use heal:(\n";
	}
}

// SpeedUp attempts to increase the player's speed stat through successful dice rolls
int Role::speedUp(int successRate, HANDLE hConsole, WORD colorSettings)
{
	system("cls");
	cout << "|| ¡´ Since your skill is speedUp, you need to roll THREE dice.\n"
		<< "|| ¡´ You have " << focus << " focus, do you want to use focus? (Y/N):";
	char input = _getch();
	cout << input << endl;

	int successDice = 0;

	if (input == 'Y') {
		cout << "|| ¡´ How many focus you're going to use: ";
		input = _getch();
		while (input - '0' > focus || isalpha(input)) {
			cout << input;
			cout << endl << "|| ¡´ invalid amount, enter again: ";
		}
		int a = input - '0';
		setFocus(getFocus() - a); // Reduce focus by chosen amount
		cout << input << "\n|| ¡´ Press any key to roll the dice!\n";
		input = _getch();
		rollingAnime();

		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		cout << "|| ¡´ Result: ";

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
			if (successRate >= 30) { successRate -= 30; } // Adjust for angryDebuff
		}

		cout << "|| ¡´ Result: ";
		colorSettings = BACKGROUND_GREEN;
		colorSettings |= BACKGROUND_RED;
		SetConsoleTextAttribute(hConsole, colorSettings);

		for (int i = 0; i < 2; i++) { // Roll two dice
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

	if (successDice) { // If successful
		cout << "\n|| ¡´ You can use speedUp!\n";
		speedUpCool++;
		setSpeed(getSpeed() * 2);
		if (getSpeed() > 100) { setSpeed(100); }
		cout << "|| ¡´ Your speed now is " << getSpeed() << "!\n";
	}
	else { // If failed
		cout << successDice;
		cout << "\n|| ¡´ So sad, you failed to use speedUp:(\n";
	}

	return getSpeed();
}

// HammerSplash is a passive skill that applies extra damage to all enemies
void Role::hammerSplash(vector<Enemy*>& es, int pHarm, int eAbsorption, HANDLE hConsole, WORD colorSettings)
{
	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED;
	SetConsoleTextAttribute(hConsole, colorSettings);

	cout << "\n|| ¡´ You activated the PASSIVE SKILL hammer-splash!\n"
		<< "|| ¡´ You can cause extra 50% damage on each enemy!\n\n";
	char input = _getch();

	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	// Calculate harm and absorption for each enemy
	for (int i = 0; i < es.size(); i++) {
		if (!es[i]->getIsDead()) { // Check if the enemy is not dead
			eAbsorption = es[i]->getMDefense() / (es[i]->getMDefense() + 50);
			pHarm = floor(pAttack * 0.5 * (1 - eAbsorption)); // Calculate additional harm
			cout << "\n|| ¡´ You caused " << pHarm << " damage on " << i + 1 << "'s enemy!\n";
			es[i]->setVitality(es[i]->getVitality() - pHarm); // Apply damage
		}
	}
	cout << endl;
}

bool isCombatOver(Role& player, vector<Enemy*> es)
{
	// Check if player has fled or is dead
	if (!player.getInBattle()) { return true; }
	if (player.getVitality() <= 0) {
		cout << "\n\n/** You are dead. **/\n";
		player.setDead(true);
		return true;
	}
	for (int i = 0; i < 3; i++)
	{
		// Continue combat if at least one enemy is alive
		if (!es[i]->getIsDead()) { return false; }
	}

	return true;
}

// Display the current status of all enemies and the player
void Role::showAllStatus(vector<Enemy*>& es)
{
	cout << "/** The following shows the battle field **/\n\n";

	// Print out status for each enemy
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

	// showStatus(); // Display player's status
}

// this function return a bool which represent if the user has chosen a valid skill
// also, it updates variable curSkill, curSkill is the chosen skill which is going to be used in battle phase
bool Role::chooseSkill()
{
	system("cls");
	cout << "||=====================================||\n";
	cout << "||===== It's your turn to attack! =====||\n";
	cout << "||=====================================||\n\n";
	// showStatus();
	cout << "\n||===== Here's avalible equipment =====||\n";
	for (auto& pair : skillEquipMap) {
		if (avalEquip[pair.second] && pair.second != "other") { // Check if the equipment exists
			// Map equipment to skill
			cout << pair.second << " -> " << pair.first << endl;
		}
	}

	cout << "\n\n|| ¡´ ENTER TO CHOOSE YOUR SKILL ¡´ ||\n";

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings |= BACKGROUND_INTENSITY;

	// Display currently chosen skill
	for (int i = 0; i < skillVec.size(); i++) {
		// Highlight the chosen skill in red
		if (i == numSkill) { colorSettings = FOREGROUND_RED; }
		else { colorSettings = FOREGROUND_INTENSITY; }
		SetConsoleTextAttribute(hConsole, colorSettings);
		cout << skillVec[i] << std::endl;
	}

	// Reset color settings for the last output
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	char input = _getch();

	if (input == 72 && numSkill > 0) { // If up is clicked
		numSkill--;
	}
	else if (input == 80 && numSkill < skillVec.size() - 1) { // down is clicked
		numSkill++;
	}
	else if (input == 13) { // If enter is clicked
		// Check if the required equipment for this skill is available
		if (avalEquip[skillEquipMap[skillVec[numSkill]]]) {
			if (speedUpCool != 0 && skillVec[numSkill] == "speedUp") {
				cout << "/!! You have to wait " << speedUpCool << " round to use speedUp!\n";
				input = _getch();
				return false;
			}
			else {
				curSkill = skillVec[numSkill];
				return true;
			}
		}
		else { // If the player doesn't have the corresponding equipment, the skill is invalid
			cout << "You don't have the cooresponding equipment to use this skill!\n";
			cout << "/** Press any key to continue **/\n";
			input = _getch();
		}
	}

	// Keep while loop running if no valid skill is chosen
	return false;
}

// Allow the player to choose an item to use before attacking
void Role::chooseItem()
{
	system("cls");
	cout << "||=====================================||\n";
	cout << "||===== It's your turn to attack! =====||\n";
	cout << "||=====================================||\n\n\n";

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;

	// Highlight the chosen skill
	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED;
	SetConsoleTextAttribute(hConsole, colorSettings);
	cout << "|| ¡´ You've chosen " << skillVec[numSkill] << " ¡´ || \n\n";

	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings);

	char input;

	if (godsBeard == 0 && goldenRoot == 0) { // Check if no items are available
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
			if (input == '\r') { // If ENTER is pressed to use godsBeard
				if (vitality > 75) { vitality = 100; } // Ensure vitality does not exceed maximum
				else { vitality += 25; }
				godsBeard--;

				// Highlight the updated vitality
				colorSettings = BACKGROUND_GREEN;
				colorSettings |= BACKGROUND_RED;
				SetConsoleTextAttribute(hConsole, colorSettings);
				cout << "|| ¡´ You've used godsBeard! ¡´ || \n";
				cout << "|| ¡´ Your vitality is : " << vitality << " ¡´ || \n\n";
				colorSettings = BACKGROUND_INTENSITY;
				colorSettings = FOREGROUND_INTENSITY;
				SetConsoleTextAttribute(hConsole, colorSettings);
			}
		}

		if (goldenRoot) {
			cout << "|| ¡´ You have " << goldenRoot << " goldenRoot.\n";
			cout << "|| ¡´ Click ENTER to apply ONE ";
			colorSettings = FOREGROUND_RED;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "goldenRoot.";
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
			cout << "\n|| ¡´ Or click any key otherwise.\n";

			input = _getch();
			if (input == '\r') { // If ENTER is pressed to use goldenRoot
				focus += 3;
				goldenRoot--;
				// Highlight the updated focus
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

// Allow the player to choose an enemy to attack
int Role::chooseEnemy(vector<Enemy*>& es)
{
	system("cls");
	cout << "||=====================================||\n";
	cout << "||===== It's your turn to attack! =====||\n";
	cout << "||=====================================||\n\n\n";
	cout << "|| ¡´ Now, you need to choose ONE enemy to attak.\n"
		"|| ¡´ The following shows all status of each alive enemy.\n\n";

	for (int i = 0; i < es.size(); i++) {
		if (!es[i]->getIsDead()) {  // Ensure the enemy is not dead
			cout << "the " << i + 1 << "'s enemy: \n";
			es[i]->showStatus();
			cout << endl << endl;
		}
	}

	cout << "Enter the number of enemy that you want to attack: ";
	char num = _getch();
	int a = num - '0' - 1;

	while (a > 2) { // Validate input range
		cout << num;
		cout << "\nInvalid input, enter again:";
		num = _getch();
		a = num - '0' - 1;
	}

	while (es[a]->getIsDead()) { // Ensure chosen enemy is alive
		cout << num;
		cout << "\nThis enemy is dead, choose again:";
		num = _getch();
		a = num - '0' - 1;
	}
	cout << num << endl;

	cout << "\n/** Click any key to continue... **/";
	num = _getch();

	// Return the index of the chosen enemy
	return a;
}

// Attack a specific enemy based on chosen skill and enemy number
void Role::attackEnemy(vector<Enemy*>& es, int enemyNum, int angryDebuff)
{
	system("cls");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	cout << "||=====================================||\n";
	cout << "||===== It's your turn to attack! =====||\n";
	cout << "||=====================================||\n\n\n";

	char input;
	int successRate = hitRate;

	// Temporarily save the original stats for restoration after the attack
	int tempPA = pAttack, tempPD = pDefense, tempMA = mAttack, tempMD = mDefense, tempS = speed;
	cout << "|| ¡´ The following are your activated equipment and attack power: \n\n";

	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED;
	SetConsoleTextAttribute(hConsole, colorSettings);

	// Check and apply each piece of equipment
	for (auto& pair : equipFuncList) {
		if (avalEquip[pair.first]) {
			cout << pair.first << " ";

			// Player can activate multiple pieces of equipment at once
			for (int i = 0; i < avalEquipNum[pair.first]; i++) {
				if (pair.first == "woodenSword") {
					woodenSword();
				}
				else if (pair.first == "giantHammer") {
					giantHammer();
				}
				else if (pair.first == "woodenShield") {
					woodenShield();
				}
				else if (pair.first == "robe") {
					robe();
				}
				else if (pair.first == "holyGrail") {
					holyGrail();
				}
				else if (pair.first == "ritualSword") {
					ritualSword();
				}
				else if (pair.first == "plateArmor") {
					plateArmor();
				}
			}

			// Display the quantity of each piece of equipment
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
		if (avalEquip["giantHammer"]) {
			hammerSplash(es, pHarm, eAbsorption, hConsole, colorSettings);
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
		tempS = speedUp(successRate, hConsole, colorSettings);
	}

	// Check the status of each enemy after the attack
	for (int i = 0; i < es.size(); i++) {
		if (es[i]->getVitality() <= 0) {
			if (!es[i]->getInBattle()) {
				cout << "\n|| ¡´ " << i + 1 << "'s enemy is already dead!\n";
			}
			else {
				cout << "\n|| ¡´ " << i + 1 << "'s enemy is dead! You earn 700 coins and a godsBeard as prizes!\n";
				money += 700;
				godsBeard++;
				es[i]->setVitality(0); // Set vitality to 0
				es[i]->setDead(true);
				es[i]->setInBattle(false);
			}
		}
		else {
			cout << "\n|| ¡´ The following is the new status of " << i + 1 << "'s enemy:\n";
			es[i]->showStatus();
		}
	}

	if (!angryDebuff) { angryDebuff--; }

	// Since items activate every time before attacking,
	// all stats must return to original status after each round,
	setSpeed(tempS);
	setPA(tempPA);
	setPD(tempPD);
	setMA(tempMA);
	setMD(tempMD);

	cout << "\n/** Your turn is over. Click any key to the combat field.\n";
	input = _getch();
}

// Conduct the entire combat sequence
bool Role::combat()
{
	// Set up enemies
	vector<Enemy*> es;
	for (int i = 0; i < 3; i++) {
		Enemy* enem = new Enemy();
		es.push_back(enem);
	}

	// Begin battle phase
	inBattle = true;
	char input;

	// Combat loop
	while (!isCombatOver(*this, es))
	{
		system("cls");

		// Display battle field status before taking a turn to attack
		showAllStatus(es);
		cout << "/** Click any key to continue **/\n";
		input = _getch();

		/*
	   * First! Get the priority of every entity!
	   */

	   // Order array helps set the order of attack, e.g., {3, 1, 2, 0}
	   // 0-2 are enemy indexes, 3 is the player
		int order[4] = { 0,1,2,3 };
		int pri = 0;

		// {index, priority}
		pair<int, double> priority[4] = { {0, 0.0}, {1, 0.0}, {2, 0.0}, {3, 0.0} };
		pair<int, double> newPriority[4] = { {0, 0.0}, {1, 0.0}, {2, 0.0}, {3, 0.0} };

		// Determine order based on priority
		for (int i = 0; i < 4; i++) {

			// If it's the player
			if (priority[i].first == 3) {
				pri = (double)(round + 1) / (double)getSpeed();
				newPriority[i] = make_pair(3, pri);
			}
			else if (!es[priority[i].first]->getIsDead()) { // If it's an alive enemy
				pri = (double)(es[priority[i].first]->round + 1) / (double)es[priority[i].first]->getSpeed();
				newPriority[i] = make_pair(priority[i].first, pri);
			}
			else { // If the enemy is dead
				pri = -1;
				newPriority[i] = make_pair(priority[i].first, pri);
			}
		}
		copy(newPriority, newPriority + 4, priority);

		// Sort the priority array using a lambda function as the comparator
		std::sort(priority, priority + 4, [](const auto& a, const auto& b) {
			return a.second < b.second;
			});

		/*
		* Second! Start the attack phase!
		*/

		// Initialize all harm and debuff settings
		int pHarm = 0, mHarm = 0, absorption = 0, enemyNum = -1;
		bool UseAngryDebuff = false;

		// Battle phase, take turns
		for (auto& pair : priority) {

			// Print out only one entity on the console
			system("cls");

			// Enemy's turn to attack the player
			if (pair.first != 3) { // Not the player's turn
				if (pair.second != -1) { // If not dead
					cout << "||=====================================||\n";
					cout << "||=== It's E" << pair.first + 1 << "'s turn to roll dice! ====||\n";
					cout << "||=====================================||\n\n\n";

					// Calculate harm on player based on enemy attack type
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
								<< "|| ¡¶ You didn't get the angry debuff!\n";
						}
						break;
					case 2:
						mHarm = es[pair.first]->shockBlast(es[pair.first]->getHitRate());
						cout << "|| ¡¶ It used MAGIC ATTACK!! \n";
						if (avalEquip["PlateArmor"]) {
							cout << "|| ¡´ Since you've activated the PlateArmor,\n|| ¡´ You have the passive skill FORTIFY, which reduce 10% damage!\n";
							mHarm *= 0.9;
						}
						cout << "|| ¡¶ DAMAGE: " << mHarm << "\n";
						break;
					case 3:
						mHarm = es[pair.first]->heal(es[pair.first]->getHitRate());  // attack 
						cout << "|| ¡¶ It used HEAL!! \n";;
						if (mHarm != 0) {
							es[pair.first]->setVitality(es[pair.first]->getVitality() + mHarm);
							cout << "|| ¡¶ This enemy gain " << mHarm << " vitality!\n";
							// In case mHarm will cause damage on player
							mHarm = 0;
						} // Self heal
						else {
							cout << "|| ¡¶ But it didn't re-fill its vitality.\n";
						}
						break;

					default:
						cout << "|| ¡¶ You are lucky! \n"
							"|| ¡¶ This enemy failed to have any valid dice!\n";
					}

					es[pair.first]->round++;

					// Update player status
					if (UseAngryDebuff) {
						UseAngryDebuff = false;
						angryDebuff += 3;
						cout << "|| ¡¶ You've gotten ANGRY DEBUFF!!\n";
						cout << "|| ¡´ You have angry debuff: " << angryDebuff << endl;
					}
					else {
						// Check if it's mAttack or pAttack
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

					// Initialize the damage after enemy's attack
					pHarm = 0;
					mHarm = 0;

					if (vitality <= 0) {
						cout << "\n|| ¡´ Your vitality is less than 0, but you can still finish this round.\n";
					}

					cout << "\n/** Press Enter to continue... **/\n";
					input = _getch();
					while (input != '\r') {
						input = _getch();
					}
				}
			} // End of enemy round
			else { // Player's turn to attack

				// For showing the chosen skill, and remember the skill name
				numSkill = 0;

				// Run until a valid skill is chosen
				while (1) {
					if (chooseSkill()) { break; }
				}

				// Check flee option
				if (curSkill == "flee") {
					if (flee()) {
						cout << "You've flee successfully, click any key to exit.";
						input = _getch();
						return true;
					}
					cout << "You filed to roll a valid dice. Press any key to keep fighting!\n";
					input = _getch();
				}
				else {
					// Choose item before combat
					chooseItem();

					if (curSkill != "heal" && curSkill != "speedUp") {
						// Choose an enemy to attack
						enemyNum = chooseEnemy(es);
						Sleep(700);
					}
				}
				attackEnemy(es, enemyNum, angryDebuff);

			} // End of player round
		}

		cout << "/** This is the end of this round, click any key to continue... **/";
		if (speedUpCool != 0 && curSkill != "speedUp") { speedUpCool--; }
		input = _getch();
	}

	// initialize all buff/debuff and delete equipment
	angryDebuff = 0;
	for (auto& pair : avalEquipNum) {
		avalEquip[pair.first] = false;
		avalEquipNum[pair.first] = 0;
		if (equipNum[pair.first] == 0) {
			auto it = find(equip.begin(), equip.end(), pair.first);
			equip.erase(it);
		}
	}
	actiEquip = 0;
	speedUpCool = 0;

	cout << "\n/** This is the end of the combat, press any key to back to main map... **/\n";
	input = _getch();

	// If the player flees, return true; if all enemies died, return false
	if (!getInBattle()) { return true; }
	else {
		return false;
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
	char input = _getch();

	while (true) {
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

			// Use red to emphasize the player is so poor that can't buy this item
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			WORD colorSettings = FOREGROUND_RED;
			SetConsoleTextAttribute(hConsole, colorSettings);
			if (!canBuy) { cout << "You don't have enough money\n"; }
			colorSettings = FOREGROUND_INTENSITY;
			SetConsoleTextAttribute(hConsole, colorSettings);
		}
		else if (GetAsyncKeyState(0x45)) { // Click E to exit
			break;
		}

		// Prevent high CPU usage
		system("pause");
		// Sleep(100);
	}
}

void Role::showChoosenItem(int num, int maxWidth, map<string, int>& bought)
{
	system("cls");  // Clear console

	cout << "\n======== Your wallet. ========\n" << endl;
	cout << "money: " << money << endl << endl;

	cout << "/** Click space to buy items.               **/" << endl;
	cout << "/** Use up and down arrows to choose items. **/\n" << endl;
	cout << "\n======== Items in sold. ========\n" << endl;
	cout << "Item" << setw(maxWidth - 4) << "Price" << endl;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings |= BACKGROUND_INTENSITY;

	// Print out the chosen equip
	for (int i = 0; i < itemsName.size(); i++)
	{
		// Turn the chosen equip to red word
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

	int total = 0;  // Calculate the total spend

	if (bought.size() != 0)
	{
		for (auto& pair : bought)
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

	// Money is enough and the hasn't bought the item yet
	if (itemsName[numOfEquip].second <= money)
	{
		// Spend money
		money -= itemsName[numOfEquip].second;

		// Add item to bag
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

		// Wait for input
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
		else if (input == '+' || input == 78) // Click + to activate the equipment
		{
			// If the chosen one is teleport, then it should not be activated
			// A function is needed to use this equipment
			if (equip[numOfEquip] == "teleportScroll") {
				teleportScroll();
				equipNum[equip[numOfEquip]]--;
			}
			else if (equipNum[equip[numOfEquip]] > 0 && actiEquip < ACTEQUIPLIM) { // This equipment is enough to use
				// Activate the equipment
				avalEquip[equip[numOfEquip]] = true;
				if (avalEquipNum.find(equip[numOfEquip]) == avalEquipNum.end()) { avalEquipNum[equip[numOfEquip]] = 1; }
				else { avalEquipNum[equip[numOfEquip]]++; }
				equipNum[equip[numOfEquip]]--;

				actiEquip++;
			}

			showChoosen(numOfEquip, maxWidth);
		}
		else if (input == '-' || input == 74) // Click - to un-activate the equipment
		{
			// The equipment can be found in avalEquipNum
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
	system("cls");  // Clear console

	cout << "/** Click enter to activate quipments.          **/" << endl;
	cout << "/** Use up and down arrows to choose quipments. **/\n" << endl;
	cout << "\n======== Your equipments. ========\n" << endl;
	cout << "Equipment" << setw(maxWidth - 9) << "Amount" << endl;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings |= BACKGROUND_INTENSITY;

	// Print out chosen equip
	for (int i = 0; i < equip.size(); i++) {
		// Turn the chosen equip to red word
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
			// Highlight the skill
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

void Role::teleportScroll()
{
	system("cls");

	int x, y; // Location of destination

	cout << "|| ¡´ You use teleportScroll!\n|| ¡´ Now you can be transported to anywhere on the map!\n";
	cout << "\n|| ¡´ The following are enemies' location info:\n\n";

	// Show enemies' locations
	for (Enemy* e : enemies) {
		Position pos = e->getPos();
		cout << "(" << pos.x << ", " << pos.y << ") ";
	}
	cout << endl << endl;

	cout << "|| ¡´ The following are shops' location info:\n\n";

	// Show shops' location
	for (Shop* s : shops) {
		Position pos = s->getPos();
		cout << "(" << pos.x << ", " << pos.y << ") ";
	}
	cout << endl << endl;

	// Get user input
	cout << "|| ¡´ Enter the location of your destination, use space to seperate (x, y):\n";
	cin >> x >> y;
	while (y > MAP_HEIGHT || x > MAP_WIDTH || x < 0 || y < 0 || gMap[y][x] != ROAD) {
		cout << "|| ¡´ The location is invalid, please enter again (x, y):\n";
		cin >> x >> y;
	}
	cout << "|| ¡´ This place is safe! Press anykey to teleport!\n";
	Position pos(x, y);
	position = pos;
	char input = _getch();
}

void Role::showStatus()
{
	cout << "||============= Your status =============||\n\n";
	cout << "|| ¡´ Vitality: " << setw(10) << getVitality() << '\n'
		<< "|| ¡´ focus: " << setw(13) << getFocus() << '\n'
		<< "|| ¡´ speed: " << setw(13) << getSpeed() << '\n'
		<< "|| ¡´ hitRate: " << setw(11) << getHitRate() << '\n'
		<< "|| ¡´ physical Attack: " << setw(3) << getPAttack() << '\n'
		<< "|| ¡´ magic Attack: " << setw(6) << getMDefense() << '\n'
		<< "|| ¡´ physical Defense: " << setw(2) << getPDefense() << '\n'
		<< "|| ¡´ magic Defense: " << setw(5) << getMDefense() << '\n'
		<< "|| ¡´ godSbeard: " << setw(9) << godsBeard << '\n'
		<< "|| ¡´ goldenRoot: " << setw(8) << goldenRoot << '\n'
		<< "|| ¡´ Money: " << setw(13) << money << '\n';
}

// Handle steps taken by throwing dice
void Role::throwDice()
{
	cout << "||=====================================||\n";
	cout << "||=== It's your turn to roll dice! ====||\n";
	cout << "||=====================================||\n\n\n";
	cout << "|| ¡´ You have " << getFocus() << " focus left.\n";
	cout << "|| ¡´ Do you want to use focus? (Y/N): ";

	char input = _getch();
	while (input != 'Y' && input != 'N') {
		cout << input << endl;
		cout << "Invalid input, enter again: ";
		input = _getch();
	}
	if (input == 'Y') {
		cout << input << "\n|| ¡´ How many points of focus you want to use?\n|| ¡´ The maximun usage is 9.\n|| ¡´ Enetr here: ";
		input = _getch();

		// Ensure that input is a number
		while (!isdigit(input)) {
			cout << input;
			cout << "\n|| ¡´ Input is not valid, please enter a number.\n";
			input = _getch();
		}

		int useFocus = input - '0';
		cout << useFocus << endl;

		while (useFocus > focus) {
			cout << "You don't have enough focus, enter again or enter 0 to continue: ";
			input = _getch();
			useFocus = input - '0';
		}

		focus -= useFocus;
		cout << "|| ¡´ You use " << input << " focus, press Enter to start rolling: ";

		while (input != '\r') {
			input = _getch();
		}
		showRollDice(useFocus);
	}
	else if (input == 'N') {
		cout << input << "\nPress Enter to start rolling! \n";

		while (input != '\r') {
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

