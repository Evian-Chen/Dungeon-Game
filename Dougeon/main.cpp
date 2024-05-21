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

int main()
{
    initializeBoard();

    /*
    std::vector<Trigger*> gTriggers;
    
    for (int i = 0; i < 2; i++) {
		Trigger* trigger = new Trigger();
		Position tPos = getValidRandomPos();
		validFlags[tPos.y][tPos.x] = false;
		trigger->setPos(tPos);
		gTriggers.push_back(trigger);
	}
    */

    // add 15 shops
    for (int i = 0; i < 15; i++) 
    {
        Shop* shop = new Shop();
        Position pos = randPosGenerator();
        shop->setPos(pos);
        shops.push_back(shop);
    }

    // add 20 enemies
    for (int i = 0; i < 20; i++)
    {
        Enemy* enemy = new Enemy();
        Position pos = randPosGenerator();
        enemy->setPos(pos);
        enemies.push_back(enemy);
    }

    // add 10 random events
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
    * �q�o�̶}�l�ץ�
    * ���ܦL�X�a�Ϫ��覡
    ************************/


    Role player1(3, 5);

    // �ΨӺ޲z��L��J���A���}�C
    bool gKeyState[ValidInput::INVALID];
    for (int i = 0; i < ValidInput::INVALID; i++) {
        gKeyState[i] = false;
    }

    setUpMap(shops, enemies, roles, randoms);

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
