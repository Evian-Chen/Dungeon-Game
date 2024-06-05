/***********************************************************************
? File: RandomEvent.cpp
? Description: This  file declares functions for handling random
				events that can occur during gameplay.
***********************************************************************/

#include <iostream>
#include <string>
#include <conio.h>       // for _getch()
#include <windows.h>      // for Sleep
#include "RandomEvent.h"

using namespace std;

HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE); // Handle for console output
WORD cSettings = FOREGROUND_INTENSITY;  // Color setting for console text

// Function to display subtitle text character by character with a delay
void runSubtitle(string str)
{
	for (char c : str)
	{
		cout << c;
		Sleep(50); // Delay between characters in milliseconds
	}
}

// Function to display a subtitle animation with background color change
int subtitleAnimation()
{
	cSettings = BACKGROUND_RED; // Set background color to red
	SetConsoleTextAttribute(con, cSettings); // Apply color settings
	system("cls"); // Clear the console screen

	string sub =
		"\n"
		" ¢i¢i¢i¢i¢i¢i       ¢i       ¢i      ¢i  ¢i¢i¢i¢i¢i      ¢i¢i¢i¢i    ¢i     ¢i    ¢i¢i¢i¢i¢i  ¢i     ¢i  ¢i¢i¢i¢i¢i  ¢i     ¢i  ¢i¢i¢i¢i¢i¢i¢i \n"
		" ¢i     ¢i     ¢i ¢i      ¢i¢i     ¢i  ¢i    ¢i    ¢i    ¢i   ¢i¢i   ¢i¢i    ¢i¢i¢i¢i¢i  ¢i     ¢i  ¢i¢i¢i¢i¢i  ¢i     ¢i  ¢i¢i¢i¢i¢i¢i¢i \n"
		" ¢i    ¢i     ¢i   ¢i     ¢i ¢i    ¢i  ¢i     ¢i  ¢i      ¢i  ¢i ¢i ¢i ¢i    ¢i      ¢i     ¢i  ¢i      ¢i¢i    ¢i     ¢i    \n"
		" ¢i¢i¢i¢i¢i     ¢i¢i¢i¢i¢i¢i¢i    ¢i  ¢i   ¢i  ¢i      ¢i ¢i      ¢i  ¢i  ¢i  ¢i    ¢i¢i¢i¢i    ¢i   ¢i   ¢i¢i¢i¢i   ¢i ¢i   ¢i     ¢i    \n"
		" ¢i ¢i       ¢i     ¢i    ¢i   ¢i  ¢i  ¢i     ¢i  ¢i      ¢i  ¢i     ¢i    ¢i¢i¢i¢i    ¢i   ¢i   ¢i¢i¢i¢i   ¢i ¢i   ¢i     ¢i    \n"
		" ¢i  ¢i     ¢i       ¢i   ¢i    ¢i ¢i  ¢i    ¢i    ¢i    ¢i   ¢i     ¢i    ¢i        ¢i ¢i    ¢i      ¢i  ¢i  ¢i     ¢i    \n"
		" ¢i   ¢i    ¢i        ¢i  ¢i     ¢i¢i  ¢i¢i¢i¢i¢i      ¢i¢i¢i¢i    ¢i     ¢i    ¢i¢i¢i¢i¢i     ¢i     ¢i¢i¢i¢i¢i  ¢i   ¢i ¢i     ¢i    \n\n";

	// Display the animation with color changes and delays
	for (int i = 0; i < 4; i++)
	{
		Sleep(500);
		cout << sub;
		Sleep(500);
		system("cls");
		cSettings = BACKGROUND_INTENSITY;
	}
	cout << sub;
	cSettings = BACKGROUND_INTENSITY; // Reset background color
	cSettings = FOREGROUND_INTENSITY; // Reset text color
	SetConsoleTextAttribute(con, cSettings); // Apply reset colors

	string sub2 = "Unfortunately, You've triggered a random event:(\nNow, \nyou can choose to draw a magic card or an opportunity card.\n"
		"If you choose magic card, you may become stronger...\n"
		"And if you choose opportunity card, you may become richer...\n"
		"Press 1 to get magic card, press 2 otherwise: ";
	runSubtitle(sub2);

	char input = _getch(); // Get user input without echoing to the console
	while (input != '1' && input != '2') {
		cout << input << endl;
		cout << "Invalid input, enter again: ";
		input = _getch();
	}
	return input - '0'; // Convert character input to integer  
}

// Function to simulate generating a result with animation
void generatingAnime()
{
	cout << "\n\n||=== generating the result... ====||\n";
	cout << "|| ";

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorSettings = FOREGROUND_INTENSITY;
	colorSettings = BACKGROUND_GREEN;
	colorSettings |= BACKGROUND_RED; // Combine green and red for a unique color
	SetConsoleTextAttribute(hConsole, colorSettings);
	for (int i = 0; i < 31; i++)
	{
		cout << ' ';
		Sleep(100); // Simulate progress bar with delays
	}

	colorSettings = BACKGROUND_INTENSITY;
	colorSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, colorSettings); // Reset color settings

	cout << " ||\n\n";
}

// Function to handle the magic card choice
void magic(Role& player)
{
	string s = "You chose magic card.\n"
		"You are going to throw a dice with number 1-6,\n"
		"Based on the number, you will recieve a mystry gift...\n"
		"Now, press any key to throw the dice!\n";
	runSubtitle(s); // Display message with subtitle effect

	char input = _getch();
	generatingAnime(); // Simulate dice throw animation

	int chance = rand() % 6;
	switch (chance)
	{
	case(0):
		runSubtitle("Re-fill focus!!\n");
		player.setFocus(3); // Set player's focus to 3
		break;
	case(1):
		runSubtitle("You gain 2 godsBeard!!\n");
		player.godsBeard += 2; // Increase player's godsBeard by 2
		break;
	case(2):
		runSubtitle("You gain 2 goldenRoot!!\n");
		player.goldenRoot += 2; // Increase player's goldenRoot by 2
		break;
	case(3):
		runSubtitle("Regen!!\n");
		player.setVitality(100); // Restore player's vitality to 100
		break;
	case(4):
		runSubtitle("You gain 30 hitRate!!\n");
		player.setHitRate(player.getHitRate() + 10); // Increase player's hit rate by 10
		break;
	case(5):
		runSubtitle("You gain 20 speed!!\n");
		player.setSpeed(player.getSpeed() + 20); // Increase player's speed by 20
		break;
	default:
		break;
	}
	cout << "\n\n/** Press eny key to exit. **/\n";
	input = _getch();
}

// Function to handle the opportunity card choice for a random event
void oppor(Role& player)
{
	string s = "You chose opportunity card.\n"
		"You are going to play paper-sissor-stone with me,\n"
		"If you win, you can get a big amount of money from me...\n"
		"Now, press 1 to go with paper, 2 with sissor, 3 with stone: \n";
	runSubtitle(s); // Display message with subtitle effect

	// Get user input (1, 2, or 3) for their choice
	char input = _getch();
	cout << input << endl;

	// Generate a random number (0 or 1) to represent the game's outcome
	if (rand() % 2 == 0)
	{
		runSubtitle("You win!!\nI'll give you a 1000 as a prize!\nGo check your backpack!\n");
		player.money += 1000; // Increase player's money by 1000
	}
	else
	{
		runSubtitle("You lose:(\nBut I'll still give 50 coins:)\n");
		player.money += 50; // Increase player's money by 50
	}
	cout << "\n\n/** Press eny key to exit. **/\n";
	input = _getch();
}

// Prompt user to press any key to exit the opportunity card functionality
void RandomEvent::randEvent(Role& player)
{
	// play animation and get an input
	int cardNum = subtitleAnimation();

	// show input
	cout << cardNum << endl;

	switch (cardNum)
	{
	case(1):
		magic(player);
		break;
	case(2):
		oppor(player);
		break;
	default:
		break;
	}
}
