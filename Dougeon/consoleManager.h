#pragma once

#include "Role.h"
#include <windows.h>
#include <conio.h>
#include "allConstants.h"

char gMap[MAP_HEIGHT][MAP_WIDTH];
bool openBag = false, showStatus = false;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum ValidInput
{
    // move
    EW = 0,
    ES = 1,
    EA = 2,
    ED = 3,

    EESC = 5,

    // open backpack
    EE = 6,

    // check role status
    EQ = 7,

    INVALID,
};

void printChr(char ch)
{
    WORD colorSettings = FOREGROUND_INTENSITY;

    switch (ch)
    {
    case ROAD:
        colorSettings |= BACKGROUND_GREEN;                  // yellow background
        colorSettings |= BACKGROUND_RED;
        break;

    case SHOP:
        colorSettings |= BACKGROUND_RED;                    // light yellow background
        colorSettings |= BACKGROUND_GREEN;
        colorSettings |= BACKGROUND_INTENSITY;
        break;

    case BLOCKED:
        colorSettings |= BACKGROUND_INTENSITY;
        break;

    case ENEMY:
        colorSettings |= FOREGROUND_RED;                    // red word
        colorSettings |= BACKGROUND_GREEN;                  // yellow background
        colorSettings |= BACKGROUND_RED;
        break;

    case PLAYER_ICON:
        colorSettings |= FOREGROUND_BLUE;                   // blue word for player icon
        colorSettings |= BACKGROUND_GREEN;                  // yellow background
        colorSettings |= BACKGROUND_RED;
        break;

    default:
        break;
    }

    SetConsoleTextAttribute(hConsole, colorSettings);
    cout << ch;

    colorSettings = BACKGROUND_INTENSITY;                   // set back to default color
    colorSettings = FOREGROUND_INTENSITY;
    SetConsoleTextAttribute(hConsole, colorSettings);
}

void setUpMap()
{
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            gMap[y][x] = ROAD;
        }
    }

    gMap[10][10] = SHOP;
    gMap[15][5] = ENEMY;
}

void printMap(int playerX, int playerY, int visionRange) {
    system("cls"); // 清除屏幕

    // 計算視野範圍內的左上角和右下角
    int startX = max(0, playerX - visionRange);
    int startY = max(0, playerY - visionRange);
    int endX = min(MAP_WIDTH - 1, playerX + visionRange);
    int endY = min(MAP_HEIGHT - 1, playerY + visionRange);

    // 打印視野範圍內的地圖
    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            if (x == playerX && y == playerY) { // 檢查是否為玩家位置
                printChr(PLAYER_ICON);
            }
            else {
                printChr(gMap[y][x]);
            }
        }
        cout << endl;
    }

    cout << "Current location: " << "(" << playerX+1 << "," << playerY+1 << ")";
}

void keyUpdate(bool key[])
{
    for (int i = 0; i < ValidInput::INVALID; i++) {
        key[i] = false;
    }
    char input = _getch();
    switch (input) {
    case 'w':
        key[ValidInput::EW] = true;
        break;
    case 's':
        key[ValidInput::ES] = true;
        break;
    case 'a':
        key[ValidInput::EA] = true;
        break;
    case 'd':
        key[ValidInput::ED] = true;
        break;
    case 'e':
        key[ValidInput::EE] = true;
        break;
    case 'q':
        key[ValidInput::EQ] = true;
        break;
    case 27:
        key[ValidInput::EESC] = true;
        break;
    default:
        break;
    }
}



/*
// 這邊之後應該要改成用render，用陣列管理每個腳色，直接用for loop update
*/
void update(bool key[], Role& player)
{
    // 清除版面
    system("CLS");

    Position delta;

    // 是否有input, 只能有一個bool是true
    bool hasInput = false;
    if (key[ValidInput::EW]) {
        delta -= Position(0, 1);
        hasInput = true;
    }
    else if (key[ValidInput::ES]) {
        delta += Position(0, 1);
        hasInput = true;
    }
    else if (key[ValidInput::EA]) {
        delta = delta - Position(1, 0);
        hasInput = true;
    }
    else if (key[ValidInput::ED]) {
        delta = delta + Position(1, 0);
        hasInput = true;
    }
    else if (key[ValidInput::EE]) {
        // the bag is already open, back to map
        if (openBag) {
            printMap(player.getPosition().x, player.getPosition().y, 10);
        }
        openBag = !openBag;
    }
    else if (key[ValidInput::EQ]) {
        // the bag is already open, back to map
        if (showStatus) {
            printMap(player.getPosition().x, player.getPosition().y, 10);
        }
        showStatus = !showStatus;
    }
    else {
        bool allInvalid = true;
        for (int i = 0; i < ValidInput::INVALID; i++) {
            if (key[i]) {
                allInvalid = false;
                break;
            }
        }
        if (allInvalid)
            std::cout << "invalid input\n";
    }

    // do something according to the input
    if (hasInput) {
        player.move(delta);
        printMap(player.getPosition().x, player.getPosition().y, 10);
    }
    else if (openBag) {
        player.showBag();
    }
    else if (showStatus) {
        player.showStatus();
    }
}

