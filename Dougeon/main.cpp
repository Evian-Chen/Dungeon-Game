/*****************************************************************//**
 * File : main.cpp
 * Description : This program is a text-based game implemented in C++.
 *               It simulates a game environment where the player controls a character and interacts with
 *               various elements such as enemies, shops, hot springs, and random events on a map.
 *               The player's goal is to navigate through the map and manage resources effectively to survive and progress in the game.
 *  *********************************************************************/
#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <string>
#include "Position.h"
#include "Role.h"
#include "consoleManager.h"
#include "RandomEvent.h"
#include "mapElement.h"

using namespace std;

// Define the external vectors to hold multiple objects
vector<Enemy*> enemies;	//vector for enemies
vector<Shop*> shops;	//vector for shops
vector<RandomEvent*> randoms;	//vector for ramdom event
vector<HotSpring*> hotSprings;	//vector for hot spring

// global variables
const double gTimeLog = 0.003;

//ASCII art at the start of the game
void startArt() {
	cout << "      ___           ___           ___                                  ___           ___     \n";
	cout << "     /\\__\\         /\\  \\         /\\  \\                                /\\  \\         /\\__\\    \n";
	cout << "    /:/ _/_       /::\\  \\       /::\\  \\                  ___          \\:\\  \\       /:/ _/_   \n";
	cout << "   /:/ /\\__\\     /:/\\:\\  \\     /:/\\:\\__\\                /\\__\\          \\:\\  \\     /:/ /\\__\\  \n";
	cout << "  /:/ /:/  /    /:/  \\:\\  \\   /:/ /:/  /               /:/  /      ___ /::\\  \\   /:/ /:/ _/_ \n";
	cout << " /:/_/:/  /    /:/__/ \\:\\__\\ /:/_/:/__/___            /:/__/      /\\  /:/\\:\\__\\ /:/_/:/ /\\__\\\n";
	cout << " \\:\\/:/  /     \\:\\  \\ /:/  / \\:\\/:::::/  /           /::\\  \\      \\:\\/:/  \\/__/ \\:\\/:/ /:/  /\n";
	cout << "  \\::/__/       \\:\\  /:/  /   \\::/~~/~~~~           /:/\\:\\  \\      \\::/__/       \\::/_/:/  / \n";
	cout << "   \\:\\  \\        \\:\\/:/  /     \\:\\~~\\               \\/__\\:\\  \\      \\:\\  \\        \\:\\/:/  /  \n";
	cout << "    \\:\\__\\        \\::/  /       \\:\\__\\            ___    \\:\\__\\ ___  \\:\\__\\        \\::/  /   \n";
	cout << "     \\/__/         \\/__/____     \\/__/           /\\  \\    \\/__//\\__\\  \\/__/         \\/__/    \n";
	cout << "                      |:|  |        ___          \\:\\  \\       /:/ _/_                        \n";
	cout << "                      |:|  |       /\\__\\          \\:\\  \\     /:/ /\\  \\                       \n";
	cout << "                    __|:|  |      /:/__/      _____\\:\\  \\   /:/ /::\\  \\                      \n";
	cout << "                   /\\ |:|__|____ /::\\  \\     /::::::::\\__\\ /:/__\\/\\:\\__\\                     \n";
	cout << "                   \\:\\/:::::/__/ \\/\\:\\  \\__  \\:\\~~\\~~\\/__/ \\:\\  \\ /:/  /                     \n";
	cout << "                    \\::/~~/~      ~~\\:\\/\\__\\  \\:\\  \\        \\:\\  /:/  /                      \n";
	cout << "                     \\:\\~~\\          \\::/  /   \\:\\  \\        \\:\\/:/  /                       \n";
	cout << "                      \\:\\__\\         /:/  /     \\:\\__\\        \\::/  /                        \n";
	cout << "                       \\/__/         \\/__/       \\/__/         \\/__/                         \n";
	cout << "\n                                NTUST OOP 2024 Final Project" << endl;
	cout << "\n                                 Press E to start the game." << endl;
	Sleep(500);
}

int main() {
	//variable for input
	char input = -1;

	//display the start screen
	startArt();
	while (input != 'E') {
		input = _getch();
	}
	system("cls");

	initializeBoard();

	// add 15 shops in the map
	for (int i = 0; i < 15; i++) {
		Shop* shop = new Shop();
		Position pos = randPosGenerator();
		shop->setPos(pos);
		shops.push_back(shop);
	}

	// add 20 hot springs in the map
	for (int i = 0; i < 20; i++) {
		HotSpring* hotSpring = new HotSpring();
		Position pos = randPosGenerator();
		hotSpring->setPos(pos);
		hotSprings.push_back(hotSpring);
	}

	// add 20 enemies in the map
	for (int i = 0; i < 20; i++) {
		Enemy* enemy = new Enemy();
		Position pos = randPosGenerator();
		enemy->setPos(pos);
		enemies.push_back(enemy);
	}

	// add 10 random events in the map
	for (int i = 0; i < 10; i++) {
		RandomEvent* random = new RandomEvent();
		Position pos = randPosGenerator();
		random->setPos(pos);
		randoms.push_back(random);
	}

	Role player1(3, 5);

	// Array to manage the keyboard input state
	bool gKeyState[ValidInput::INVALID];
	for (int i = 0; i < ValidInput::INVALID; i++) {
		gKeyState[i] = false;
	}

	setUpMap();
	printMap(player1);

	// Variables for game loop
	clock_t startT, endT;
	startT = clock();
	endT = clock();

	// Run the game loop
	while (!gKeyState[ValidInput::EESC]) {
		// Compute the time lap
		double timeFrame = (double)(endT - startT) / CLOCKS_PER_SEC;

		// Execute the game loop
		if (timeFrame >= gTimeLog) {
			if (update(gKeyState, player1)) { break; }
			startT = clock();
		}

		// Update the key state
		keyUpdate(gKeyState);
		endT = clock();
	}

	cout << "\nPress esc to close the game.";
	input = _getch();

	system("pause");
	return 0;
}
