#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <math.h>
#include <ctime>
#include <string>
#include "Position.h"
#include "Role.h"
#include "consoleManager.h"
#include "RandomEvent.h"

using namespace std;

// global variables
const double gTimeLog = 0.003;

// player vector
vector<Role*> roles;

// enemy vector
vector<Enemy*> enemies;

// shop vector
vector<Shop*> shops;

// random event
vector<RandomEvent*> randoms;

// hot spring vector
vector<HotSpring*> hotSprings;

void startArt()
{
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

int main()
{
    char input = -1;

    startArt();
    while (input != 'E') {
        input = _getch();
    }
    system("cls");

    initializeBoard();

    // add 15 shops
    for (int i = 0; i < 15; i++) 
    {
        Shop* shop = new Shop();
        Position pos = randPosGenerator();
        shop->setPos(pos);
        shops.push_back(shop);
    }

    // add 20 hot springs
    for (int i = 0; i < 20; i++)
    {
        HotSpring* hotSpring = new HotSpring();
        Position pos = randPosGenerator();
        hotSpring->setPos(pos);
        hotSprings.push_back(hotSpring);
    }

    // add 20 enemies
    for (int i = 0; i < 20; i++)
    {
        Enemy* enemy = new Enemy();
        Position pos = randPosGenerator();
        enemy->setPos(pos);
        enemies.push_back(enemy);
    }

    // add 20 random events
    for (int i = 0; i < 10; i++)
    {
        RandomEvent* random = new RandomEvent();
        Position pos = randPosGenerator();
        random->setPos(pos);
        randoms.push_back(random);
    }

    // add 1 role
    for (int i = 0; i < 1; i++)
    {
        Role* role = new Role();
        Position pos = randPosGenerator();
        role->setPos(pos);
        roles.push_back(role);
    }



    /***********************
    * 從這裡開始修正
    * 改變印出地圖的方式
    ************************/


    Role player1(3, 5);

    // 用來管理鍵盤輸入狀態的陣列
    bool gKeyState[ValidInput::INVALID];
    for (int i = 0; i < ValidInput::INVALID; i++) {
        gKeyState[i] = false;
    }

    setUpMap(shops, enemies, roles, randoms, hotSprings);

    printMap(player1);

    // Variable for game loop
    clock_t startT, endT;
    startT = clock();
    endT = clock();

    // Run the game loop
    while (!gKeyState[ValidInput::EESC]) {
        // Compute the time lap
        double timeFrame = (double)(endT - startT) / CLOCKS_PER_SEC;

        // Execute the game loop
        if (timeFrame >= gTimeLog) {
            update(gKeyState, player1);
            startT = clock();
        }

        // Update the key
        keyUpdate(gKeyState);
        endT = clock();
    }

    system("pause");
    return 0;
}
