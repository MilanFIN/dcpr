#ifndef MENU_H
#define MENU_H

#include <tonc.h>
#include "render.h"
#include "dungeon.h"
#include "textures.h"
#include "raycaster.h"
#include "toplayer.h"

#define MINIMAPVISIBLERADIUS 4

extern EWRAM_DATA int keyX;
extern EWRAM_DATA int keyXAdd;
extern EWRAM_DATA int keyY;
extern EWRAM_DATA int keyYAdd;

extern int difficulty;
extern int randomInitialized;

/// @brief render start menu bkg with a moving icon
void renderBkg();

void renderMenuBkg();

void renderSplash();

/// @brief render "press start" screen and initialize random number generator based on user input
void renderStart();

void renderMainMenu();

int renderPlayMenu();

void renderKeysMenu();

void renderGuideMenu();


/// @brief draw pause menu l/r arrow icons
void drawArrows();

/// @brief draw primary tab of the pause menu
/// @param map game tilemap (1d buffer of 2d map)
/// @param visited tilemap of previously visited positions
/// @param playerX player position on tilemap
/// @param playerY -||-
void renderPause1st(int *map, char *visited, int playerX, int playerY);

/// @brief draw secondary tab of pause menu and return user input on exit
/// @return 1 for resume, 0 for switch tab, -1 for exit game
int renderPause2nd();

/// @brief Draw the pause menu tabs
/// @param map game tilemap (1d buffer of 2d map)
/// @param visited tilemap of previously visited positions
/// @param playerX player position on tilemap
/// @param playerY -||-
/// @return true: resume game, false: exit
int renderPauseMenu(int *map, char *visited, int playerX, int playerY);

void renderLevelDone();

#endif // MENU_H