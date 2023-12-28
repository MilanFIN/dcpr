#include "dungeon.h"
#include <tonc.h>

static int PAIRID = 0;
static int ID = 0;
static int MAXITERATIONS = 100;
static int MINROOMSIZE = 5;

struct Leaf {
	int active;
	int x;
	int y;
	int xend;
	int yend;
	int pairId;
	int id;
};



void divide(struct Leaf* leaves, struct Leaf* leaf, int horizontal) {
	PAIRID++;

	int xSize = leaf->xend - leaf->x;
	int ySize = leaf->yend - leaf->y;

	int xmid, ymid;
	if (xSize > MINROOMSIZE && ySize > MINROOMSIZE && ID < MAXITERATIONS) {
		xmid = qran_range(leaf->x+2, leaf->xend -1);
		ymid = qran_range(leaf->y+2, leaf->yend -1);

		if (horizontal) {

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
		else { //vertical

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
	else {
		
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

void generate(struct Leaf* leaf, struct Leaf* finalLeaves, struct Leaf* pairTree, int maxDepth, int depth) {

	int direction = qran_range(0, 2);
	struct Leaf leaves[2];
	leaves[0].active = 0;
	leaves[1].active = 0;
	divide(&leaves, leaf, direction);

	for (int i = 0; i < MAXITERATIONS; i++) {
		if (!pairTree[i].active) {
			if (leaves[0].active && leaves[1].active) {
				//printf("OK\n");
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


	if (depth < maxDepth) {
		for (int i = 0; i < 2; i++) {
			if (leaves[i].active) {

				generate(&leaves[i], finalLeaves, pairTree, maxDepth, depth+1);
			}
		}
	}
	else {
		for (int i = 0; i < MAXITERATIONS; i++) {
			if (!finalLeaves[i].active) {
				finalLeaves[i] = leaves[0];
				finalLeaves[i+1] = leaves[1];
				break;
			}
		}
		
	}

}

void getCorridor(struct Leaf* first, struct Leaf* second, int* x, int* y, int* xend, int* yend) {

	int firstMidX = (first->x + first->xend) / 2;
	int secondMidX = (second->x + second->xend) / 2;
	int firstMidY = (first->y + first->yend) / 2;
	int secondMidY = (second->y + second->yend) / 2;

	*x = firstMidX;
	*y = firstMidY;
	*xend = secondMidX;
	*yend = secondMidY;

	if (*x > *xend) {
		int temp = *x;
		*x = *xend;
		*xend = temp;
	}

	if (*y > *yend) {
		int temp = *y;
		*y = *yend;
		*yend = temp;
	}
}


void shrink(struct Leaf* leaves) {
    for (int i = 0; i < MAXITERATIONS; i++) {
        struct Leaf* leaf = &leaves[i];
        if (!leaf->active) {
            continue;
        }
        else {
            int xSize = leaf->xend - leaf->x;
            int ySize = leaf->yend - leaf->y;
            if (xSize > 5) {
                int xmid = (leaf->xend + leaf->x) / 2;
                int x = qran_range(leaf->x, xmid+1);
        		int xend = qran_range(xmid, leaf->xend+1);
        		leaf->x = x;
        		leaf->xend = xend;
            }
            if (ySize > 5) {
                int ymid = (leaf->yend + leaf->y) / 2;
                int y = qran_range(leaf->y, ymid+1);
        		int yend = qran_range(ymid, leaf->yend);
        		leaf->y = y;
        		leaf->yend = yend;
            }
        }
    }
}


void getDungeon(char* map, int mapsize, FIXED* playerX, FIXED* playerY) {
	PAIRID = 0;
	ID = 0;
	struct Leaf rootLeaf;

	rootLeaf.x = 1;
	rootLeaf.y = 1;
	rootLeaf.xend = mapsize-1;
	rootLeaf.yend = mapsize-1;

	struct Leaf finalLeaves[MAXITERATIONS];
	struct Leaf pairTree[MAXITERATIONS];

	//initialize the array of leaves that are unused
	for (int i = 0; i < MAXITERATIONS; i++) {
		finalLeaves[i].active = 0;
		pairTree[i].active = 0;
	}

	generate(&rootLeaf, &finalLeaves, &pairTree, 6, 0);
	shrink(&finalLeaves);
	
	for (int y = 0; y < mapsize; y++) {
		for (int x = 0; x < mapsize; x++) {
			map[mapsize*y+x] = 1;
		}
	}

	for (int i = 0; i < MAXITERATIONS; i++) {
		struct Leaf iter = finalLeaves[i];
		if (!iter.active) {
			continue;
		}

		for (int y = 0; y < mapsize-1; y++) {
			for (int x = 0; x < mapsize-1; x++) {
				if (x >= iter.x && y >= iter.y && x <= iter.xend && y <= iter.yend) {
					map[mapsize*y+x] = 0;
				}
			}
		}
	}

	for (int i = 0; i < MAXITERATIONS; i++) {
		struct Leaf first = pairTree[i];
		if (!first.active) {
			continue;
		}
		                				

        for (int x = first.x; x < first.xend+1; x++) {
	    	for (int y = first.y; y < first.yend+1; y++) {
	    		map[mapsize*y+x] = 0;
	    	}
	    }
	}	

	//no longer needed, as player is placed relative to the door
	
	for (int x = 0; x < mapsize; x++) {
		for (int y = 0; y < mapsize; y++) {
			if (map[mapsize*y+x] == 0) {
				*playerX = int2fx(x) + 128;
				*playerY = int2fx(y) + 128;
				return;
			}
		}
	}
	

}

