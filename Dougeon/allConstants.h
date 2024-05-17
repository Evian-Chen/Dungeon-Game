#pragma once

const int MAP_WIDTH = 140;
const int MAP_HEIGHT = 50;

extern char gMap[MAP_HEIGHT][MAP_WIDTH];

const char ROAD = '.';
const char SHOP = '$';
const char BLOCKED = '¡½';
const char ENEMY = 'E';
const char PLAYER_ICON = 'R';

const int shopX = 44;
const int shopY = 136;

const int VISIONRANGE = 7;

const int ACTEQUIPLIM = 10;