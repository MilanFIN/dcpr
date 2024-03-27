#ifndef DUNGEON_H
#define DUNGEON_H

#include <tonc.h>

#define MAPSIZE 50
#define MAXITERATIONS 500

struct Leaf
{
	int active;
	int x;
	int y;
	int xend;
	int yend;
	int pairId;
	int id;
};

extern int PAIRID;
extern int ID;
extern int MINROOMSIZE;
extern int MAXDEPTH;

extern int mapSize;

extern int MAP[MAPSIZE * MAPSIZE];
extern int UTILITYMAPDATA[MAPSIZE * MAPSIZE];
extern char VISITEDLOCATIONS[MAPSIZE * MAPSIZE];

extern EWRAM_DATA struct Leaf finalLeaves[MAXITERATIONS];
extern EWRAM_DATA struct Leaf pairTree[MAXITERATIONS];

/// @brief Splits a room into two at a random line (bsp). New rooms are within the bounds of the previous one
/// @param leaves a pointer to the newly created rooms, or the original if no split was made
/// @param leaf the room that is to be split
/// @param horizontal direction of the split
void divide(struct Leaf *leaves, struct Leaf *leaf, int horizontal);

/// @brief generates a corridor between two rooms
/// @param first one of the source rooms
/// @param second  -||-
/// @param x corridor start coordinate
/// @param y -||-
/// @param xend corridor end coordinate
/// @param yend -||-
void getCorridor(struct Leaf *first, struct Leaf *second, int *x, int *y, int *xend, int *yend);

/// @brief Recursive function, that runs a bsp for a 2d array of tiles to create a set of rooms
///			Rooms take up all available space, shrinking is done later on
/// @param leaf root leaf or "room". Should cover the entire playable area excluding outer walls
/// @param finalLeaves Results array for rooms
/// @param pairTree Results array for corridors between rooms
/// @param maxDepth max recursion depth
/// @param depth current recursion depth, eg 0 at start
void generate(struct Leaf *leaf, struct Leaf finalLeaves[MAXITERATIONS], struct Leaf pairTree[MAXITERATIONS], int maxDepth, int depth);

/// @brief loops through rooms and shrinks them randomly while preserving their connection to the relevant corridor
/// @param leaves rooms made by bsp (generate)
void shrink(struct Leaf leaves[MAXITERATIONS]);

/// @brief main interface to use the dungeon generator
/// @param map pointer to game map array, 1d buffer that contains a 2d map
/// @param mapsize map size (eg 50x50 => 50)
void getDungeon(int *map, int mapsize);

bool collisionCheck(FIXED x, FIXED y);

#endif // DUNGEON_H