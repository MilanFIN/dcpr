#include "dungeon.h"


//int MAP[32*32] = {0 };



static int PAIRID = 0;
static int ID = 0;

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
	if (xSize > 5 && ySize > 5) {
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

	for (int i = 0; i < 100; i++) {
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
		for (int i = 0; i < 100; i++) {
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

void getDungeon(int* map) {
	sqran(-256);
	PAIRID = 0;
	ID = 0;
	struct Leaf rootLeaf;

	rootLeaf.x = 1;
	rootLeaf.y = 1;
	rootLeaf.xend = 31;
	rootLeaf.yend = 31;

	struct Leaf finalLeaves[100];
	struct Leaf pairTree[100];

	//initialize the array of leaves that are unused
	for (int i = 0; i < 100; i++) {
		finalLeaves[i].active = 0;
		pairTree[i].active = 0;
	}

	generate(&rootLeaf, &finalLeaves, &pairTree, 6, 0);
	
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			map[32*y+x] = 1;
		}
	}

	for (int i = 0; i < 100; i++) {
		struct Leaf iter = finalLeaves[i];
		if (!iter.active) {
			continue;
		}

		for (int y = 0; y < 31; y++) {
			for (int x = 0; x < 31; x++) {
				if (x >= iter.x && y >= iter.y && x <= iter.xend && y <= iter.yend) {
					map[32*y+x] = 0;
				}
			}
		}
	}


    int counter = 0;
	for (int i = 0; i < 100; i++) {
		struct Leaf first = pairTree[i];
		if (!first.active) {
			continue;
		}
		                				

        for (int x = first.x; x < first.xend+1; x++) {
	    	for (int y = first.y; y < first.yend+1; y++) {
	    		map[32*y+x] = 0;
	    	}
	    }

	}

}
