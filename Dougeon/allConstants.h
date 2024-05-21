#pragma once

const int MAP_WIDTH = 140;
const int MAP_HEIGHT = 50;
const int MIN_SIZE = 9;

extern char gMap[MAP_HEIGHT][MAP_WIDTH];

const char ROAD = '.';
const char EVENT = '!';
const char SHOP = '$';
const char BLOCKED = 'X';
const char ENEMY = 'E';
const char PLAYER_ICON = 'R';

const int SHOPNUM = 5;
const int ENEMYNUM = 5;

const int VISIONRANGE = 7;

const int ACTEQUIPLIM = 10;