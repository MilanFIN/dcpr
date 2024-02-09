#include <tonc.h>
#include "render.h"
#include "dungeon.h"

#ifndef MENU_H
#define MENU_H

#define MINIMAPVISIBLERADIUS 3

/// @brief render "press start" screen and initialize random number generator based on user input
void renderStart()
{
	int seed = 0;
	while (1)
	{
		seed++;
		key_poll();
		if (key_hit(KEY_START) || key_hit(KEY_A))
		{
			break;
		}

		fillArea(0, 0, 240, 160, 7);
		writeLine("PRESS START", 11, 10, 80, 15);
		vid_flip();
	}

	sqran(seed);
}

/// @brief render level size selection screen and initialize mapsize variable on exit
void renderMenu()
{
	int size = 0;
	while (1)
	{

		key_poll();
		if (key_hit(KEY_A) || key_hit(KEY_START))
		{
			break;
		}
		if (key_hit(KEY_DOWN))
		{
			if (size < 2)
				size++;
		}
		if (key_hit(KEY_UP))
		{
			if (size > 0)
				size--;
		}

		fillArea(0, 0, 240, 160, 7);
		writeLine("LEVEL SIZE", 10, 10, 10, 15);
		writeLine("SMALL", 5, 20, 40, 15);
		writeLine("AVERAGE", 7, 20, 60, 15);
		writeLine("HUGE", 4, 20, 80, 15);

		writeLine(">", 1, 10, 40 + 20 * size, 15);
		vid_flip();
	}

	mapSize = 10 + size * 10;
}

bool checkAround(char* map, int x, int y) {
	for (int i = x - MINIMAPVISIBLERADIUS; i < x+MINIMAPVISIBLERADIUS+1; i++) {
		for (int j = y - MINIMAPVISIBLERADIUS; j < y+MINIMAPVISIBLERADIUS+1; j++) {
			if (i >= 0 && i < mapSize && j >= 0 && j < mapSize) {
				if (map[MAPSIZE * j + i] != 0) {
					return true;
				} 
			}
		}
	}
	return false;
}

void renderPauseMenu(char *map, char *visited, int playerX, int playerY)
{


	const int xOffset = 34;
	const int yOffset = 15;
	fillArea(2 * xOffset, 2 * yOffset, 2 * (MAPSIZE-1 + xOffset), 2 * (MAPSIZE  + yOffset), 0);

	vid_flip();


	FIXED sizeMultiplier = fxdiv(int2fx(MAPSIZE), int2fx(mapSize));

	for (int screenY = 0; screenY < MAPSIZE; screenY++)
	{
		for (int screenX = 0; screenX < MAPSIZE; screenX++)
		{

			int x = fx2int(fxdiv(int2fx(screenX), sizeMultiplier));
			int y = fx2int(fxdiv(int2fx(screenY), sizeMultiplier));

			FIXED xRemainder = fxsub(fxdiv(int2fx(screenX), sizeMultiplier), int2fx(x));
			FIXED yRemainder = fxsub(fxdiv(int2fx(screenY), sizeMultiplier), int2fx(y));

			int color = 2;
			if (map[MAPSIZE * y + x] == 0)
			{
				color = 132;
			}
			if (xRemainder < 128 && yRemainder < 128)
			{
				if (map[MAPSIZE * y + x] == 5)
				{
					color = 23; // door
				}
				if (playerX == x && playerY == y)
				{
					color = 15; // player
				}
			}
			if (!checkAround(visited, x, y))
			{
				color = 0;
			}
			fillArea(2 * (screenX + xOffset), 2 * (screenY + yOffset), 2 * (screenX + xOffset), 2 * (screenY + yOffset) + 1, color);
		}
	}
	vid_flip();

	while (1)
	{
		key_poll();

		if (key_hit(KEY_START))
		{
			break;
		}
	}
}

#endif // MENU_H