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

using namespace std;


// constants
const double gTimeLog = 0.033;

int main()
{
    srand(time(NULL));

    Role player1(138, 45);

    // 用來管理鍵盤輸入狀態的陣列
    bool gKeyState[ValidInput::INVALID];
    for (int i = 0; i < ValidInput::INVALID; i++) {
        gKeyState[i] = false;
    }

    setUpMap();

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
