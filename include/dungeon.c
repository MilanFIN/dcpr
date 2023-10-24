#include "dungeon.h"

static int PAIRID = 0;


struct Leaf {
	bool active;
	int x;
	int y;
	int xend;
	int yend;
	int pairId;
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

			leaves[0].active = true;
			leaves[0].x = leaf->x;
			leaves[0].y = leaf->y;
			leaves[0].xend = xmid - 1;
			leaves[0].yend = leaf->yend;
			leaves[0].pairId = PAIRID;

			leaves[1].active = true;
			leaves[1].x = xmid + 1;
			leaves[1].y = leaf->y;
			leaves[1].xend = leaf->xend;
			leaves[1].yend = leaf->yend;
			leaves[1].pairId = PAIRID;

		}
		else { //vertical

			leaves[0].active = true;
			leaves[0].x = leaf->x;
			leaves[0].y = leaf->y;
			leaves[0].xend = leaf->xend;
			leaves[0].yend = ymid - 1;
			leaves[0].pairId = PAIRID;

			leaves[1].active = true;
			leaves[1].x = leaf->x;
			leaves[1].y = ymid + 1;
			leaves[1].xend = leaf->xend;
			leaves[1].yend = leaf->yend;
			leaves[1].pairId = PAIRID;

		}

	}
	else {
		leaves[0].active = true;
		leaves[0].x = leaf->x;
		leaves[0].y = leaf->y;
		leaves[0].xend = leaf->xend;
		leaves[0].yend = leaf->yend;
		leaves[0].pairId = leaf->pairId;
	}
}

void generate(struct Leaf* leaf, struct Leaf* finalLeaves, int maxDepth, int depth) {

	int direction = qran_range(0, 2);
	struct Leaf leaves[2];
	leaves[0].active = false;
	leaves[1].active = false;
	divide(&leaves, leaf, direction);

	if (depth < maxDepth) {
		//leaves[0] = *leaf;
		//generate(&leaves[0], finalLeaves, maxDepth, depth+1);
		for (int i = 0; i < 2; i++) {
			if (leaves[i].active) {
				generate(&leaves[i], finalLeaves, maxDepth, depth+1);
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


void getDungeon(int* map) {
	sqran(-256);
	PAIRID = 0;
	struct Leaf rootLeaf;

	rootLeaf.x = 1;
	rootLeaf.y = 1;
	rootLeaf.xend = 31;
	rootLeaf.yend = 31;

	struct Leaf finalLeaves[100];

	//initialize the array of leaves that are unused
	for (int i = 0; i < 100; i++) {
		finalLeaves[i].active = false;
	}

	generate(&rootLeaf, &finalLeaves, 6, 0);
	
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			map[32*y+x] = 1;
		}
	}
	//temp taking the first half, should loop through finalleaves here instead
	rootLeaf = finalLeaves[0];
	for (int i = 0; i < 100; i++) {
		struct Leaf iter = finalLeaves[i];
		if (!iter.active) {
			continue;
		}
		for (int y = 1; y < 31; y++) {
			for (int x = 1; x < 31; x++) {
				if (x >= iter.x && y >= iter.y && x <= iter.xend && y <= iter.yend) {
					map[32*y+x] = 0;

				}
			}
		}

	}
	
	//map[25] = 1;
}