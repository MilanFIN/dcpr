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

extern EWRAM_DATA char MAP[MAPSIZE * MAPSIZE];

extern EWRAM_DATA struct Leaf finalLeaves[MAXITERATIONS];
extern EWRAM_DATA struct Leaf pairTree[MAXITERATIONS];

void divide(struct Leaf *leaves, struct Leaf *leaf, int horizontal);

void getCorridor(struct Leaf *first, struct Leaf *second, int *x, int *y, int *xend, int *yend);

void generate(struct Leaf *leaf, struct Leaf finalLeaves[MAXITERATIONS], struct Leaf pairTree[MAXITERATIONS], int maxDepth, int depth);

void shrink(struct Leaf leaves[MAXITERATIONS]);

void getDungeon(char *map, int mapsize, FIXED *playerX, FIXED *playerY);

#endif // DUNGEON_H