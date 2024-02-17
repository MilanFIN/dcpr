#ifndef MENU_H
#define MENU_H

#include <tonc.h>
#include "render.h"
#include "dungeon.h"
#include "textures.h"
#include "raycaster.h"

#define MINIMAPVISIBLERADIUS 3

extern EWRAM_DATA int keyX;
extern EWRAM_DATA int keyXAdd;
extern EWRAM_DATA int keyY;
extern EWRAM_DATA int keyYAdd;

void renderBkg();

/// @brief render "press start" screen and initialize random number generator based on user input
void renderStart();

/// @brief render level size selection screen and initialize mapsize variable on exit
void renderMenu();

bool checkAround(char *map, int x, int y);

void drawArrows();

void renderPause1st(char *map, char *visited, int playerX, int playerY);

int renderPause2nd();

bool renderPauseMenu(char *map, char *visited, int playerX, int playerY);

#endif // MENU_H