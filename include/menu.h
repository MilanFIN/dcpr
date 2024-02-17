#include <tonc.h>
#include "render.h"
#include "dungeon.h"
#include "textures.h"
#include "raycaster.h"

#ifndef MENU_H
#define MENU_H

#define MINIMAPVISIBLERADIUS 3

EWRAM_DATA int keyX = 0;
EWRAM_DATA int keyXAdd = 2;
EWRAM_DATA int keyY = -32;
EWRAM_DATA int keyYAdd = 2;

void renderBkg()
{

	drawFlat(TEXTURES, 1, 0, 0, 64, 64, 0, TEXTURESIZE);
	drawFlat(TEXTURES, 1, 64, 0, 64, 64, 0, TEXTURESIZE);

	drawFlat(TEXTURES, 3, keyX, keyY, 32, 32, 0, TEXTURESIZE);

	keyX += keyXAdd;
	keyY += keyYAdd;

	if (keyX > 86 || keyX < 0)
	{
		keyXAdd = -keyXAdd;
		keyX += keyXAdd;
	}
	if (keyY > 64 || keyY < -32)
	{
		keyYAdd = -keyYAdd;
		keyY += keyYAdd;
	}
}

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

		renderBkg();
		writeLine("PRESS START", 11, 10, 140, 15);
		vid_flip();
	}

	sqran(seed);
}

/// @brief render level size selection screen and initialize mapsize variable on exit
void renderMenu()
{
	int size = 0;
	key_poll();

	while (1)
	{

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

		fillArea(0, 128, 240, 160, 16);
		drawFlat(TEXTURES, 1, 0, 0, 64, 64, 0, TEXTURESIZE);
		drawFlat(TEXTURES, 1, 64, 0, 64, 64, 0, TEXTURESIZE);

		writeLine("LEVEL SIZE", 10, 10, 10, 15);
		writeLine("SMALL", 5, 20, 40, 15);
		writeLine("MEDIUM", 6, 20, 60, 15);
		writeLine("LARGE", 5, 20, 80, 15);

		writeLine(">", 1, 10, 40 + 20 * size, 15);
		vid_flip();
		key_poll();
	}

	mapSize = 30 + size * 10;
}

bool checkAround(char *map, int x, int y)
{

	if (map[MAPSIZE * y + x] != 0)
	{
		return true;
	}

	return false;
}

void drawArrows()
{
	drawFlat(TEXTURES, 17, 100, 5, 16, 16, 0, TEXTURESIZE);
	drawFlatMirrored(TEXTURES, 17, 4, 5, 16, 16, 0, TEXTURESIZE);
}

void renderPause1st(char *map, char *visited, int playerX, int playerY)
{
	castRays();
	const int xOffset = 34;
	const int yOffset = 15;

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

	drawArrows();

	writeLine("START TO", 8, 23, 1, 15);
	writeLine("CONTINUE", 8, 23, 16, 15);

	vid_flip();
}

int renderPause2nd()
{
	int selection = 0;

	while (1)
	{
		key_poll();

		if (key_hit(KEY_RIGHT) || key_hit(KEY_LEFT) || key_hit(KEY_R) || key_hit(KEY_L))
		{
			return 0;
		}
		if (key_hit(KEY_A))
		{
			if (selection == 0)
			{
				return 1;
			}
			else if (selection == 1)
			{
				return -1;
			}
		}
		if (key_hit(KEY_DOWN))
		{
			selection += 1;
			selection = CLAMP(selection, 0, 2);
		}
		if (key_hit(KEY_UP))
		{
			selection -= 1;
			selection = CLAMP(selection, 0, 2);
		}

		castRays();
		// drawEntities();
		drawArrows();
		writeLine("RESUME", 6, 38, 50, 15);
		writeLine("QUIT", 4, 38, 66, 15);
		writeLine(">", 1, 27, 50 + selection * 16, 15);

		vid_flip();
	}
}

bool renderPauseMenu(char *map, char *visited, int playerX, int playerY)
{
	while (1)
	{
		renderPause1st(map, visited, playerX, playerY);

		while (1)
		{
			key_poll();

			if (key_hit(KEY_RIGHT) || key_hit(KEY_LEFT) || key_hit(KEY_R) || key_hit(KEY_L))
			{
				int selection = renderPause2nd();
				if (selection == 1)
				{
					return true;
				}
				if (selection == -1)
				{
					return false;
				}
				break;
			}

			if (key_hit(KEY_START))
			{
				return true;
			}
		}
	}
}

#endif // MENU_H