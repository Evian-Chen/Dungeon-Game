/***********************************************************************
 * File: allConstants.h
 * Description: This program is to initialize all constants core attributes in the game.
***********************************************************************/
#pragma once

// Map Dimensions
const int MAP_WIDTH = 140;
const int MAP_HEIGHT = 50;

// Minimum size requirement
const int MIN_SIZE = 9;

// External declaration of the game map
extern char gMap[MAP_HEIGHT][MAP_WIDTH];

// Tile symbols representing different game elements
const char ROAD = '.';
const char EVENT = '!';
const char HOTSPRING = 'H';
const char SHOP = '$';
const char BLOCKED = 'X';
const char ENEMY = 'E';
const char PLAYER_ICON = 'R';
const char FINAL = 'F';

// Number of shops and enemies
const int SHOPNUM = 5;
const int ENEMYNUM = 5;

// Role vision range
const int VISIONRANGE = 7;

// Action equipment limit
const int ACTEQUIPLIM = 10;
