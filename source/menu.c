#include "menu.h"
#include "textures.h"
#include "dungeon.h"
#include "audio.h"
#include "timer.h"

int keyX = 0;
int keyXAdd = 1;
int keyY = -32;
int keyYAdd = 1;

void renderBkg()
{
	const int startX = CLAMP(2*keyX - 4, 0, 238);
	const int startY = CLAMP((keyY + 32) - 4, 0, 138);
	const int endX = CLAMP(2*keyX + 64 +4, 0, 238);
	const int endY = CLAMP((keyY + 32) + 32 + 4, 0, 138);

	fillArea(startX, startY, endX, endY, 16);

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

void renderStart()
{

	int seed = 0;
	// renderBkg();

	while (1)
	{
		if (key_hit(KEY_START) || key_hit(KEY_A))
		{
			break;
		}

		key_poll();
		renderBkg();
		if (seed < 2)
		{
			writeLine("PRESS START", 11, 10, 140, 15);
		}
		seed++;

		vid_flip();
	}

	playSound(7);
	sqran(seed);
}

void renderMenu()
{
	int size = 0;

	while (1)
	{
		key_poll();

		drawFlat(TEXTURES, 1, 0, 0, 64, 64, 0, TEXTURESIZE);
		drawFlat(TEXTURES, 1, 64, 0, 64, 64, 0, TEXTURESIZE);
		fillArea(0, 128, 240, 136, 1);

		drawFlat(TEXTURES, 1, 0, 136, 64, 64, 0, TEXTURESIZE);
		drawFlat(TEXTURES, 1, 64, 136, 64, 64, 0, TEXTURESIZE);

		writeLine("LEVEL SIZE", 10, 10, 10, 15);
		writeLine("SMALL", 5, 20, 40, 15);
		writeLine("MEDIUM", 6, 20, 60, 15);
		writeLine("LARGE", 5, 20, 80, 15);

		writeLine(">", 1, 10, 40 + 20 * size, 15);
		vid_flip();

		if (key_hit(KEY_A) || key_hit(KEY_START))
		{
			playSound(10);
			break;
		}
		if (key_hit(KEY_DOWN))
		{
			if (size < 2)
			{
				size++;
				playSound(9);
			}
		}
		if (key_hit(KEY_UP))
		{
			if (size > 0)
			{
				size--;
				playSound(9);
			}
		}
	}

	mapSize = 30 + size * 10;
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

	FIXED remainderLimit = fxdiv(int2fx(mapSize), int2fx(MAPSIZE));

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
			if (xRemainder < remainderLimit && yRemainder < remainderLimit)
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
			// hide unvisited areas
			if (visited[MAPSIZE * y + x] == 0)
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
	int time = readTimer();
	int selection = 0;

	while (1)
	{
		vid_vsync();

		key_poll();

		if (key_hit(KEY_RIGHT) || key_hit(KEY_LEFT) || key_hit(KEY_R) || key_hit(KEY_L))
		{
			playSound(9);
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
			playSound(9);
			selection += 1;
			selection = CLAMP(selection, 0, 2);
		}
		if (key_hit(KEY_UP))
		{
			playSound(9);
			selection -= 1;
			selection = CLAMP(selection, 0, 2);
		}

		castRays();
		// drawEntities();
		drawArrows();

		const char s1 = (time % 60 % 10) + '0';
		const char s0 = (time % 60 / 10) + '0';
		const char m1 = (time / 60) + '0';
		const char m0 = (time / 60 / 10) + '0';
		const char h1 = (time / 3600) + '0';
		const char h0 = (time / 3600 / 10) + '0';

		char timeLabel[8] = {h0, h1, ':', m0, m1, ':', s0, s1};
		writeLine("TIME", 4, 42, 40, 15);
		writeLine(timeLabel, 8, 25, 56, 15);

		writeLine("RESUME", 6, 38, 88, 15);
		writeLine("QUIT", 4, 38, 104, 15);
		writeLine(">", 1, 27, 88 + selection * 16, 15);

		vid_flip();
	}
}

bool renderPauseMenu(char *map, char *visited, int playerX, int playerY)
{
	pauseTimer();

	playSound(9);

	while (1)
	{
		renderPause1st(map, visited, playerX, playerY);

		while (1)
		{
			key_poll();

			if (key_hit(KEY_RIGHT) || key_hit(KEY_LEFT) || key_hit(KEY_R) || key_hit(KEY_L))
			{
				playSound(9);
				int selection = renderPause2nd();
				if (selection == 1)
				{
					playSound(9);
					resumeTimer();
					return true;
				}
				if (selection == -1)
				{
					playSound(9);
					return false;
				}
				break;
			}

			if (key_hit(KEY_START))
			{
				playSound(9);
				resumeTimer();
				return true;
			}
		}
	}
}

void renderLevelDone()
{
	int time = readTimer();

	drawFlat(TEXTURES, 1, 0, 0, 64, 64, 0, TEXTURESIZE);
	drawFlat(TEXTURES, 1, 64, 0, 64, 64, 0, TEXTURESIZE);
	fillArea(0, 128, 240, 136, 1);

	drawFlat(TEXTURES, 1, 0, 136, 64, 64, 0, TEXTURESIZE);
	drawFlat(TEXTURES, 1, 64, 136, 64, 64, 0, TEXTURESIZE);

	writeLine("LEVEL DONE", 10, 15, 20, 15);

	const char s1 = (time % 60 % 10) + '0';
	const char s0 = (time % 60 / 10) + '0';
	const char m1 = (time / 60) + '0';
	const char m0 = (time / 60 / 10) + '0';
	const char h1 = (time / 3600) + '0';
	const char h0 = (time / 3600 / 10) + '0';

	char timeLabel[8] = {h0, h1, ':', m0, m1, ':', s0, s1};
	writeLine("TIME", 4, 42, 40, 15);
	writeLine(timeLabel, 8, 25, 56, 15);

	writeLine("PRESS A", 7, 28, 80, 15);
	writeLine("TO CONTINUE", 11, 10, 96, 15);

	vid_flip();

	while (1)
	{
		key_poll();
		if (key_hit(KEY_A))
		{
			break;
		}
	}
}