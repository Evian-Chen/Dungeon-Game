#pragma once

#include "Position.h"
#include "Role.h"
#include "Shop.h"
#include "Enemy.h"
#include "RandomEvent.h"
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
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
    EB = 6,

    // check role status
    EQ = 7,

    INVALID,
};

Position randPosGenerator()
{
    int x = rand() % MAP_HEIGHT;
    int y = rand() % MAP_WIDTH;
    while (gMap[x][y] != ROAD)
    {
        x = rand() % MAP_HEIGHT;
        y = rand() % MAP_WIDTH;
        cout << x << ' ' << y;
    }
    Position pos(x, y);
    return pos;
}

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

    case EVENT:
        colorSettings |= FOREGROUND_RED;                    // red word for event icon
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

// for maze
void divide(int x, int y, int width, int height) {
    if (width < MIN_SIZE || height < MIN_SIZE) return;

    bool horizontal = width < height;
    if (width == height) {
        horizontal = rand() % 2;
    }

    if (horizontal) {
        // Divide horizontally
        int wy = y + (rand() % (height - MIN_SIZE + 1) + MIN_SIZE / 2);
        int px = x + (rand() % width);

        for (int i = x; i < x + width; ++i) {
            gMap[wy][i] = BLOCKED;
        }
        gMap[wy][px] = ROAD;

        divide(x, y, width, wy - y);
        divide(x, wy + 1, width, y + height - wy - 1);
    }
    else {
        // Divide vertically
        int wx = x + (rand() % (width - MIN_SIZE + 1) + MIN_SIZE / 2);
        int py = y + (rand() % height);

        for (int i = y; i < y + height; ++i) {
            gMap[i][wx] = BLOCKED;
        }
        gMap[py][wx] = ROAD;

        divide(x, y, wx - x, height);
        divide(wx + 1, y, x + width - wx - 1, height);
    }
}

void initializeBoard() {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            gMap[y][x] = ROAD;
        }
    }
}

void setUpMap(vector<Shop*> shops, vector<Enemy*> enemies, vector<Role*> roles, vector<RandomEvent*> randoms)
{
    srand(1283568); // for devide (making maze)
    // initializeBoard();

    // Add the border walls
    for (int x = 0; x < MAP_WIDTH; ++x) {
        gMap[0][x] = BLOCKED;
        gMap[MAP_HEIGHT - 1][x] = BLOCKED;
    }
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        gMap[y][0] = BLOCKED;
        gMap[y][MAP_WIDTH - 1] = BLOCKED;
    }

    divide(1, 1, MAP_WIDTH - 2, MAP_HEIGHT - 2);

    // add shops
    for (int i = 0; i < shops.size(); i++)
    {
        gMap[shops[i]->getX()][shops[i]->getY()] = shops[i]->getIcon();
    }

    // add enemies
    for (int i = 0; i < enemies.size(); i++)
    {
        gMap[enemies[i]->getX()][enemies[i]->getY()] = enemies[i]->getIcon();
    }

    // add random
    for (int i = 0; i < randoms.size(); i++)
    {
        gMap[randoms[i]->getX()][randoms[i]->getY()] = randoms[i]->getIcon();
    }

    // for testing
    gMap[6][3] = randoms[0]->getIcon();
    gMap[2][5] = enemies[0]->getIcon();

}

void printInfo(Role& player)
{
    // player info
    cout << "\n|| ● Player\n";
    cout << "|| ● Position: (" << player.getPosition().x << ", " << player.getPosition().y << ")\n";
    cout << "|| ● Vitality: ";

    WORD colorSettings = BACKGROUND_BLUE;
    SetConsoleTextAttribute(hConsole, colorSettings);
    for (int i = 0; i < player.getVitality() / 10; i++)
    {
        cout << ' ';
    }
    colorSettings = BACKGROUND_INTENSITY;
    colorSettings = FOREGROUND_INTENSITY;
    SetConsoleTextAttribute(hConsole, colorSettings);
    for (int i = 0; i < 10 - player.getVitality() / 10; i++)
    {
        cout << ' ';
    }

    cout << " || " << player.getVitality() << "/100\n";
    cout << "|| ● Speed: " << player.getSpeed() << endl;
    cout << "|| ● Focus: " << player.getFocus() << endl;
    cout << "|| ● Avalible steps left: " << player.getAvalStep() << endl;

    cout << endl << endl;

    // width = 80
    cout << " _______________________________________________\n";
    cout << "|||||||||||||||||||||||||||||||||||||||||||||||||\n";
    cout << "||                                             ||\n";
    cout << "||    Press W,S,A,D to move your character.    ||\n";
    cout << "||          Press B to open your bag.          ||\n";
    cout << "||        Press E to check your status.        ||\n";
    cout << "||_____________________________________________||\n";
    cout << "|||||||||||||||||||||||||||||||||||||||||||||||||\n";
}

void printMap(Role& player) {
    int playerX = player.getPosition().x, playerY = player.getPosition().y;

    system("cls"); // 清除屏幕

    // 計算視野範圍內的左上角和右下角
    int startX = max(0, playerX - VISIONRANGE);
    int startY = max(0, playerY - VISIONRANGE);
    int endX = min(MAP_WIDTH - 1, playerX + VISIONRANGE);
    int endY = min(MAP_HEIGHT - 1, playerY + VISIONRANGE);

    // 打印視野範圍內的地圖
    for (int y = 0; y <= VISIONRANGE*2; y++) {
        for (int x = 0; x <= VISIONRANGE*6; x++) {
            if (startY + y >= MAP_HEIGHT || startX + x >= MAP_WIDTH)
            {
                cout << 'X';
            }

            else if (startX + x == playerX && startY + y == playerY) { // 檢查是否為玩家位置
                printChr(PLAYER_ICON);
            }
            else {
                printChr(gMap[startY + y][startX + x]);
            }
        }
        cout << " ||\n";
    }

    for (int i = 0; i <= VISIONRANGE * 6 + 3; i++)
    {
        cout << '=';
    }

    

    cout << "\n\n";
    printInfo(player);
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
    case 'b':
        key[ValidInput::EB] = true;
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
    else if (key[ValidInput::EB]) {
        openBag = !openBag;
    }
    else if (key[ValidInput::EQ]) {
        // the status list is already open, back to map
        if (showStatus) {
            printMap(player);
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
            printMap(player);
            
            // use red color
            WORD colorSettings = FOREGROUND_RED;
            SetConsoleTextAttribute(hConsole, colorSettings);
            std::cout << "\nInvalid input!!\n";
            colorSettings = FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(hConsole, colorSettings);
    }

    // do something according to the input
    if (hasInput) {
        player.move(delta);
        printMap(player);
    }
    else if (openBag) {
        player.showBag();
        printMap(player);
        openBag = !openBag;
    }
    else if (showStatus) {
        player.showStatus();
    }
}
