#include <iostream>
#include <string>
#include <conio.h>       // for _getch()
#include <windows.h>      // for Sleep
#include "RandomEvent.h"

using namespace std;

HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
WORD cSettings = FOREGROUND_INTENSITY;

void runSubtitle(string str)
{
	for (char c : str)
	{
		cout << c;
		Sleep(50);
	}
}

int subtitleAnimation()
{
	cSettings = BACKGROUND_RED;
	SetConsoleTextAttribute(con, cSettings);
	system("cls");
	
	string sub = 
		"\n"
		" ¢i¢i¢i¢i¢i¢i       ¢i       ¢i      ¢i  ¢i¢i¢i¢i¢i      ¢i¢i¢i¢i    ¢i     ¢i    ¢i¢i¢i¢i¢i  ¢i     ¢i  ¢i¢i¢i¢i¢i  ¢i     ¢i  ¢i¢i¢i¢i¢i¢i¢i \n"
		" ¢i     ¢i     ¢i ¢i      ¢i¢i     ¢i  ¢i    ¢i    ¢i    ¢i   ¢i¢i   ¢i¢i    ¢i¢i¢i¢i¢i  ¢i     ¢i  ¢i¢i¢i¢i¢i  ¢i     ¢i  ¢i¢i¢i¢i¢i¢i¢i \n"
		" ¢i    ¢i     ¢i   ¢i     ¢i ¢i    ¢i  ¢i     ¢i  ¢i      ¢i  ¢i ¢i ¢i ¢i    ¢i      ¢i     ¢i  ¢i      ¢i¢i    ¢i     ¢i    \n"
		" ¢i¢i¢i¢i¢i     ¢i¢i¢i¢i¢i¢i¢i    ¢i  ¢i   ¢i  ¢i      ¢i ¢i      ¢i  ¢i  ¢i  ¢i    ¢i¢i¢i¢i    ¢i   ¢i   ¢i¢i¢i¢i   ¢i ¢i   ¢i     ¢i    \n"
		" ¢i ¢i       ¢i     ¢i    ¢i   ¢i  ¢i  ¢i     ¢i  ¢i      ¢i  ¢i     ¢i    ¢i¢i¢i¢i    ¢i   ¢i   ¢i¢i¢i¢i   ¢i ¢i   ¢i     ¢i    \n"
		" ¢i  ¢i     ¢i       ¢i   ¢i    ¢i ¢i  ¢i    ¢i    ¢i    ¢i   ¢i     ¢i    ¢i        ¢i ¢i    ¢i      ¢i  ¢i  ¢i     ¢i    \n"
		" ¢i   ¢i    ¢i        ¢i  ¢i     ¢i¢i  ¢i¢i¢i¢i¢i      ¢i¢i¢i¢i    ¢i     ¢i    ¢i¢i¢i¢i¢i     ¢i     ¢i¢i¢i¢i¢i  ¢i   ¢i ¢i     ¢i    \n\n";
	for (int i = 0; i < 4; i++)
	{
		Sleep(500);
		cout << sub;
		Sleep(500);
		system("cls");
		cSettings = BACKGROUND_INTENSITY;
	}
	cout << sub;
	cSettings = BACKGROUND_INTENSITY;
	cSettings = FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(con, cSettings);

	string sub2 = "Unfortunately, You've triggered a random event:(\nNow, \nyou can choose to draw a magic card or an opportunity card.\n"
		"If you choose magic card, you may become stronger...\n"
		"And if you choose opportunity card, you may become richer...\n"
		"Press 1 to get magic card, press 2 otherwise: ";	
	runSubtitle(sub2);

	char input = _getch() - '0';
	return input;
}

void generatingAnime()
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

void magic(Role& player)
{
	string s = "You chose magic card.\n"
		"You are going to throw a dice with number 1-6,\n"
		"Based on the number, you will recieve a mystry gift...\n"
	    "Now, press any key to throw the dice!\n";
	runSubtitle(s);

	char input = _getch();
	generatingAnime();
	
	int chance = rand() % 6;
	switch (chance)
	{
	case(0):
		runSubtitle("Re-fill focus!!\n");
		player.setFocus(3);
		break;
	case(1):
		runSubtitle("You gain 2 godsBeard!!\n");
		player.godsBeard += 2;
		break;
	case(2):
		runSubtitle("You gain 2 goldenRoot!!\n");
		player.goldenRoot += 2;
		break;
	case(3):
		runSubtitle("Regen!!\n");
		player.setVitality(100);
		break;
	case(4):
		runSubtitle("You gain 30 hitRate!!\n");
		player.setHitRate(player.getHitRate() + 10);
		break;
	case(5):
		runSubtitle("You gain 20 speed!!\n");
		player.setSpeed(player.getSpeed() + 20);
		break;
	default:
		break;
	}
	cout << "\n\n/** Press eny key to exit. **/\n";
	input = _getch();
}

void oppor(Role& player)
{
	string s = "You chose opportunity card.\n"
		"You are going to play paper-sissor-stone with me,\n"
		"If you win, you can get a big amount of money from me...\n"
		"Now, press 1 to go with paper, 2 with sissor, 3 with stone: \n";
	runSubtitle(s);

	char input = _getch();
	cout << input << endl;

	if (rand() % 2 == 0)
	{
		runSubtitle("You win!!\nI'll give you a 1000 as a prize!\nGo check your backpack!\n");
		player.money += 1000;
	}
	else
	{
		runSubtitle("You lose:(\nBut I'll still give 50 coins:)\n");
		player.money += 50;
	}
	cout << "\n\n/** Press eny key to exit. **/\n";
	input = _getch();
}

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
