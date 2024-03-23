
#ifndef ENTITIES_H
#define ENTITIES_H

#include <tonc.h>
#include "utils.h"
#include "structs.h"
#include "audio.h"
#include "dungeon.h"
#include "hud.h"
#include "raycaster.h"
#include "menu.h"

#define MAXENTITYCOUNT 32

extern const int ENEMYSIZES[6];
extern const int ENEMYSPEEDS[6];
extern const int ENEMYLEVELS[6];
extern const int ENEMYTEXTURES[6];
extern const int ENEMYOFFSETS[6];


extern const int ENEMYTEXCOUNT;
extern const int goalEnemyCount;
extern const int PRUNEENEMYDISTANCE;

extern struct Entity entities[MAXENTITYCOUNT];
extern int entityOrder[MAXENTITYCOUNT];

/// @brief initialize an enemy entity
/// @param id position of the entity in entity array
/// @param x tile position (0 to mapsize)
/// @param y -||-
void initEnemy(int id, int x, int y);

/// @brief initialize an item pickup
/// @param id position in entity array
/// @param x tile position (0 to mapsize)
/// @param y
void initPickup(int type, int id, int x, int y);

/// @brief initialize "key" entity position, key is always first in entity array
/// @param x tile position (0 to mapsize)
/// @param y -||-
void initKey(int x, int y);

void initSplatter(int id);

/// @brief initialize entity array with unused values
void initEntities();

void fire(FIXED dirX, FIXED dirY);

void removeEntity(int i);

void checkEntityCollisions();

void moveEntities();

/// @brief helper for quicksort
/// @param v1
/// @param v2
/// @return
int compareDistances(const void *v1, const void *v2);

/// @brief helper for quicksort
/// @param array
/// @param low
/// @param high
/// @return
int partition(int array[], int low, int high);

/// @brief main quicksort function, used to order entities based on distance to player
/// @param array
/// @param low
/// @param high
void quickSort(int array[], int low, int high);

/// @brief sort entities based on distance to player
void sortEntities();

/// @brief draw all visible entities on screen, if not hidden by walls
void drawEntities();

void refillEnemies();

void pruneFarAwayEnemies();

#endif // ENTITIES_H