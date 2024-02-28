
#include "dungeon.h"

int PAIRID = 0;
int ID = 0;
int MINROOMSIZE = 3;
int MAXDEPTH = 25;

int mapSize = MAPSIZE;

char MAP[MAPSIZE * MAPSIZE] = {0};
EWRAM_DATA int UTILITYMAPDATA[MAPSIZE * MAPSIZE] = {0};
EWRAM_DATA char VISITEDLOCATIONS[MAPSIZE * MAPSIZE] = {0};

EWRAM_DATA struct Leaf finalLeaves[MAXITERATIONS] = {};
EWRAM_DATA struct Leaf pairTree[MAXITERATIONS] = {};

void divide(struct Leaf *leaves, struct Leaf *leaf, int horizontal)
{
	PAIRID++;

	int xSize = leaf->xend - leaf->x;
	int ySize = leaf->yend - leaf->y;

	int xmid, ymid;
	if (xSize > MINROOMSIZE && ySize > MINROOMSIZE && ID < MAXITERATIONS)
	{
		xmid = qran_range(leaf->x + 2, leaf->xend - 1);
		ymid = qran_range(leaf->y + 2, leaf->yend - 1);

		if (horizontal)
		{

			leaves[0].active = 1;
			leaves[0].x = leaf->x;
			leaves[0].y = leaf->y;
			leaves[0].xend = xmid - 1;
			leaves[0].yend = leaf->yend;
			leaves[0].pairId = PAIRID;
			leaves[0].id = ID;
			ID++;

			leaves[1].active = 1;
			leaves[1].x = xmid + 1;
			leaves[1].y = leaf->y;
			leaves[1].xend = leaf->xend;
			leaves[1].yend = leaf->yend;
			leaves[1].pairId = PAIRID;
			leaves[1].id = ID;
			ID++;
		}
		else
		{ // vertical

			leaves[0].active = 1;
			leaves[0].x = leaf->x;
			leaves[0].y = leaf->y;
			leaves[0].xend = leaf->xend;
			leaves[0].yend = ymid - 1;
			leaves[0].pairId = PAIRID;
			leaves[0].id = ID;
			ID++;

			leaves[1].active = 1;
			leaves[1].x = leaf->x;
			leaves[1].y = ymid + 1;
			leaves[1].xend = leaf->xend;
			leaves[1].yend = leaf->yend;
			leaves[1].pairId = PAIRID;
			leaves[1].id = ID;
			ID++;
		}
	}
	else
	{

		leaves[0].active = 1;
		leaves[0].x = leaf->x;
		leaves[0].y = leaf->y;
		leaves[0].xend = leaf->xend;
		leaves[0].yend = leaf->yend;
		leaves[0].pairId = leaf->pairId;
		leaves[0].id = ID;
		ID++;
	}
}

void getCorridor(struct Leaf *first, struct Leaf *second, int *x, int *y, int *xend, int *yend)
{

	int firstMidX = (first->x + first->xend) / 2;
	int secondMidX = (second->x + second->xend) / 2;
	int firstMidY = (first->y + first->yend) / 2;
	int secondMidY = (second->y + second->yend) / 2;

	*x = firstMidX;
	*y = firstMidY;
	*xend = secondMidX;
	*yend = secondMidY;

	if (*x > *xend)
	{
		int temp = *x;
		*x = *xend;
		*xend = temp;
	}

	if (*y > *yend)
	{
		int temp = *y;
		*y = *yend;
		*yend = temp;
	}
}

void generate(struct Leaf *leaf, struct Leaf finalLeaves[MAXITERATIONS], struct Leaf pairTree[MAXITERATIONS], int maxDepth, int depth)
{

	int direction = qran_range(0, 2);
	struct Leaf leaves[2];
	leaves[0].active = 0;
	leaves[1].active = 0;
	divide(leaves, leaf, direction);

	for (int i = 0; i < MAXITERATIONS; i++)
	{
		if (!pairTree[i].active)
		{
			if (leaves[0].active && leaves[1].active)
			{
				int x = 0;
				int y = 0;
				int xend = 0;
				int yend = 0;

				getCorridor(&leaves[0], &leaves[1], &x, &y, &xend, &yend);
				pairTree[i].x = x;
				pairTree[i].y = y;
				pairTree[i].xend = xend;
				pairTree[i].yend = yend;
				pairTree[i].active = 1;
			}
			break;
		}
	}

	if (depth < maxDepth)
	{
		for (int i = 0; i < 2; i++)
		{
			if (leaves[i].active)
			{

				generate(&leaves[i], finalLeaves, pairTree, maxDepth, depth + 1);
			}
		}
	}
	else
	{
		for (int i = 0; i < MAXITERATIONS; i++)
		{
			if (!finalLeaves[i].active)
			{
				finalLeaves[i] = leaves[0];
				finalLeaves[i + 1] = leaves[1];
				break;
			}
		}
	}
}

void shrink(struct Leaf leaves[MAXITERATIONS])
{
	for (int i = 0; i < MAXITERATIONS; i++)
	{
		struct Leaf *leaf = &leaves[i];
		if (!leaf->active)
		{
			continue;
		}
		else
		{
			int xSize = leaf->xend - leaf->x;
			int ySize = leaf->yend - leaf->y;
			if (xSize > 5)
			{
				int xmid = (leaf->xend + leaf->x) / 2;
				int x = qran_range(leaf->x, xmid + 1);
				int xend = qran_range(xmid, leaf->xend + 1);
				leaf->x = x;
				leaf->xend = xend;
			}
			if (ySize > 5)
			{
				int ymid = (leaf->yend + leaf->y) / 2;
				int y = qran_range(leaf->y, ymid + 1);
				int yend = qran_range(ymid, leaf->yend);
				leaf->y = y;
				leaf->yend = yend;
			}
		}
	}
}

void getDungeon(char *map, int mapsize)
{
	PAIRID = 0;
	ID = 0;
	struct Leaf rootLeaf;

	rootLeaf.x = 1;
	rootLeaf.y = 1;
	rootLeaf.xend = mapsize - 2;
	rootLeaf.yend = mapsize - 2;

	// initialize the array of leaves that are unused
	for (int i = 0; i < MAXITERATIONS; i++)
	{
		finalLeaves[i].active = 0;
		pairTree[i].active = 0;
	}

	generate(&rootLeaf, finalLeaves, pairTree, MAXDEPTH, 0);
	shrink(finalLeaves);

	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			char wallVal = 1;
			if (qran_range(0, 15) < 2)
			{
				wallVal = 2;
			}
			map[MAPSIZE * y + x] = wallVal;
		}
	}

	for (int i = 0; i < MAXITERATIONS; i++)
	{
		struct Leaf iter = finalLeaves[i];
		if (!iter.active)
		{
			continue;
		}

		for (int y = 0; y < MAPSIZE - 1; y++)
		{
			for (int x = 0; x < MAPSIZE - 1; x++)
			{
				if (x >= iter.x && y >= iter.y && x <= iter.xend && y <= iter.yend)
				{
					map[MAPSIZE * y + x] = 0;
				}
			}
		}
	}

	for (int i = 0; i < MAXITERATIONS; i++)
	{
		struct Leaf first = pairTree[i];
		if (!first.active)
		{
			continue;
		}

		for (int x = first.x; x < first.xend + 1; x++)
		{
			for (int y = first.y; y < first.yend + 1; y++)
			{
				map[MAPSIZE * y + x] = 0;
			}
		}
	}
}

/// @brief check if a point collides with a wall
/// @param x fixed point position
/// @param y fixed point position
/// @return
bool collisionCheck(FIXED x, FIXED y)
{
	if (MAP[fx2int(y) * MAPSIZE + fx2int(x)] != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}