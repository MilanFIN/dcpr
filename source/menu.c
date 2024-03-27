#include "menu.h"
#include "textures.h"
#include "dungeon.h"
#include "audio.h"
#include "timer.h"
#include "hud.h"

int keyX = 0;
int keyXAdd = 1;
int keyY = 0;
int keyYAdd = 1;

int difficulty = 0;
bool randomInitialized = false;

void renderBkg()
{
	const int startX = CLAMP(2 * keyX - 4, 0, 238);
	const int startY = CLAMP((keyY)-4, 0, 138);
	const int endX = CLAMP(2 * keyX + 64 + 4, 0, 238);
	const int endY = CLAMP((keyY) + 4 + 32, 64, 138);

	fillArea(startX, startY, endX, endY, 16);

	drawFlat(TEXTURES, 23, keyX, keyY, 32, 32, 0, TEXTURESIZE);

	keyX += keyXAdd;
	keyY += keyYAdd;

	if (keyX > 86 || keyX <= 0)
	{
		keyXAdd = -keyXAdd;
	}
	if (keyY > 96 || keyY <= 0)
	{
		keyYAdd = -keyYAdd;
	}
}

void renderMenuBkg()
{
	drawFlat(TEXTURES, 1, 0, 0, 64, 64, 0, TEXTURESIZE);
	drawFlat(TEXTURES, 1, 64, 0, 64, 64, 0, TEXTURESIZE);
	fillArea(0, 128, 240, 136, 1);
	drawFlat(TEXTURES, 1, 0, 136, 64, 64, 0, TEXTURESIZE);
	drawFlat(TEXTURES, 1, 64, 136, 64, 64, 0, TEXTURESIZE);
}

void renderSplash()
{

	fillArea(14, 24, 226, 108, 16);

	drawFlatColorFullResolution(LETTERS, 21, 34, 44, 40, 40, 15, 1, LETTERSIZE);
	drawFlatColorFullResolution(LETTERS, 20, 78, 44, 40, 40, 15, 1, LETTERSIZE);
	drawFlatColorFullResolution(LETTERS, 33, 122, 44, 40, 40, 15, 1, LETTERSIZE);
	drawFlatColorFullResolution(LETTERS, 35, 166, 44, 40, 40, 15, 1, LETTERSIZE);
}

void renderStart()
{
	startSeedTimer();
	// renderMenuBkg();
	// renderSplash();

	int init = 0;

	while (1)
	{
		key_poll();

		if (key_hit(KEY_START) || key_hit(KEY_A))
		{
			break;
		}
		if (init < 2)
		{
			fillArea(0, 0, 240, 140, 16);
			fillArea(0, 128, 240, 160, 16);

			writeLine("PRESS START", 11, 10, 140, 15, true);

			init++;
		}
		renderBkg();
		vid_flip();

		// VBlankIntrWait();
	}
	VBlankIntrWait();
	playSound(7);
}

void renderMainMenu()
{

	int count = 0;

	int selection = 0;
	while (1)
	{
		key_poll();

		if (count < 2)
		{
			renderMenuBkg();
			fillArea(18, 4, 222, 32, 16);
			writeLine("MAIN MENU", 9, 20, 15, 15, true);
			fillArea(44, 45, 200, 115, 16);

			writeLine("PLAY", 4, 37, 55, 15, true);
			writeLine("KEYS", 4, 37, 75, 15, true);
			writeLine("GUIDE", 5, 37, 95, 15, true);
			count++;
		}
		fillArea(54, 50, 70, 110, 16);

		writeLine(">", 1, 27, 55 + 20 * selection, 15, true);

		if (key_hit(KEY_DOWN))
		{
			VBlankIntrWait();
			playSound(9);
			if (selection < 2)
			{
				selection++;
			}
		}
		else if (key_hit(KEY_UP))
		{
			VBlankIntrWait();
			playSound(9);
			if (selection > 0)
			{
				selection--;
			}
		}

		if (key_hit(KEY_A) || key_hit(KEY_START))
		{
			VBlankIntrWait();
			playSound(9);

			if (selection == 0)
			{
				if (renderPlayMenu())
					return;
			}
			else if (selection == 1)
			{
				renderKeysMenu();
			}
			else if (selection == 2) {
				renderGuideMenu();
			}
			count = 0;
			VBlankIntrWait();
			playSound(5);
		}

		vid_flip();
		VBlankIntrWait();
	}
}

bool renderPlayMenu()
{

	while (1)
	{
		int size = 0;
		int diff = 0;

		int count = 0;

		while (1)
		{
			key_poll();

			if (count < 2)
			{
				renderMenuBkg();
				fillArea(18, 4, 222, 32, 16);
				writeLine("LEVEL SIZE", 10, 16, 15, 15, true);
				fillArea(44, 45, 200, 115, 16);

				writeLine("SMALL", 5, 37, 55, 15, true);
				writeLine("MEDIUM", 6, 37, 75, 15, true);
				writeLine("LARGE", 5, 37, 95, 15, true);
				count++;
			}

			fillArea(54, 50, 70, 110, 16);

			writeLine(">", 1, 27, 55 + 20 * size, 15, true);

			if (key_hit(KEY_A) || key_hit(KEY_START))
			{
				VBlankIntrWait();
				playSound(9);
				mapSize = 30 + size * 10;
				break;
			}
			if (key_hit(KEY_B))
			{
				return false;
			}
			else if (key_hit(KEY_DOWN))
			{
				VBlankIntrWait();
				playSound(9);
				if (size < 2)
				{
					size++;
				}
			}
			else if (key_hit(KEY_UP))
			{
				VBlankIntrWait();
				playSound(9);
				if (size > 0)
				{
					size--;
				}
			}
			vid_flip();
			VBlankIntrWait();
		}

		count = 0;
		while (1)
		{
			key_poll();

			if (count < 2)
			{
				renderMenuBkg();
				fillArea(18, 4, 222, 32, 16);
				writeLine("DIFFICULTY", 10, 16, 15, 15, true);
				fillArea(44, 45, 200, 135, 16);

				writeLine("EASY", 4, 37, 55, 15, true);
				writeLine("NORMAL", 6, 37, 75, 15, true);
				writeLine("HARD", 4, 37, 95, 15, true);
				writeLine(";1", 2, 37, 115, 15, true);

				count++;
			}

			fillArea(54, 50, 70, 130, 16);

			writeLine(">", 1, 27, 55 + 20 * diff, 15, true);

			if (key_hit(KEY_A) || key_hit(KEY_START))
			{
				difficulty = diff;
				VBlankIntrWait();
				playSound(10);
				if (!randomInitialized)
				{
					randomInitialized = true;
					sqran(readSeedTimer());
				}
				return true;
			}
			else if (key_hit(KEY_B))
			{
				VBlankIntrWait();
				playSound(5);
				break;
			}
			else if (key_hit(KEY_DOWN))
			{
				VBlankIntrWait();
				playSound(9);
				if (diff < 3)
				{
					diff++;
				}
			}
			else if (key_hit(KEY_UP))
			{
				VBlankIntrWait();
				playSound(9);
				if (diff > 0)
				{
					diff--;
				}
			}
			vid_flip();
			VBlankIntrWait();
		}
	}
}

void renderKeysMenu()
{

	int count = 0;

	while (1)
	{
		key_poll();

		if (count < 2)
		{
			renderMenuBkg();
			fillArea(18, 4, 222, 32, 16);
			writeLine("KEYS", 4, 42, 15, 15, true);
			fillArea(10, 45, 230, 155, 16);

			writeLine("DPAD          MOVE", 18, 39, 55, 15, false);
			writeLine("L<R         STRAFE", 18, 39, 75, 15, false);
			writeLine("A        OPEN DOOR", 18, 39, 95, 15, false);
			writeLine("B           ATTACK", 18, 39, 115, 15, false);
			writeLine("START    PAUSE<MAP", 18, 39, 135, 15, false);

			count++;
		}

		if (key_hit(KEY_B))
		{
			return;
		}
		vid_flip();
		VBlankIntrWait();
	}
}

void renderGuideMenu()
{

	int count = 0;

	while (1)
	{
		key_poll();

		if (count < 2)
		{
			renderMenuBkg();
			fillArea(18, 4, 222, 32, 16);
			writeLine("GUIDE", 5, 38, 15, 15, true);
			fillArea(10, 45, 230, 155, 16);

			writeLine("THERE IS A KEY HIDDEN", 21, 25, 55, 15, false);
			writeLine("SOMEWHERE IN THE DUNGEON", 24, 13, 70, 15, false);
			writeLine("TRY TO FIND IT", 14, 57, 95, 15, false);
			writeLine("AFTERWARDS FIND YOUR WAY", 24, 13, 120, 15, false);
			writeLine("BACK TO TO THE DOOR", 19, 35, 135, 15, false);

			count++;
		}

		if (key_hit(KEY_B))
		{
			return;
		}
		vid_flip();
		VBlankIntrWait();
	}
}

void drawArrows()
{
	drawFlat(TEXTURES, 17, 100, 5, 16, 16, 0, TEXTURESIZE);
	drawFlatMirrored(TEXTURES, 17, 4, 5, 16, 16, 0, TEXTURESIZE);
}

void renderPause1st(int *map, char *visited, int playerX, int playerY)
{
	castRays();
	drawHands();
	const int xOffset = 34;
	const int yOffset = 10;

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

	vid_flip();
	VBlankIntrWait();
}

int renderPause2nd()
{
	int time = readTimer();

	const char s1 = (time % 60 % 10) + '0';
	const char s0 = (time % 60 / 10) + '0';
	const char m1 = (time / 60) + '0';
	const char m0 = (time / 60 / 10) + '0';
	const char h1 = (time / 3600) + '0';
	const char h0 = (time / 3600 / 10) + '0';

	char timeLabel[8] = {h0, h1, ':', m0, m1, ':', s0, s1};

	int selection = 0;
	int count = 0;

	while (1)
	{
		key_poll();

		if (count < 2)
		{
			castRays();
			drawHands();
			drawArrows();

			fillArea(40, 32, 200, 122, 16);

			writeLine("TIME", 4, 42, 40, 15, true);
			writeLine(timeLabel, 8, 25, 56, 15, true);

			writeLine("RESUME", 6, 38, 88, 15, true);
			writeLine("QUIT", 4, 38, 104, 15, true);
			count++;
		}

		if (key_hit(KEY_RIGHT) || key_hit(KEY_LEFT) || key_hit(KEY_R) || key_hit(KEY_L))
		{
			VBlankIntrWait();
			playSound(9);
			return 0;
		}
		else if (key_hit(KEY_A))
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
		else if (key_hit(KEY_DOWN))
		{
			VBlankIntrWait();
			playSound(9);
			selection += 1;
			selection = CLAMP(selection, 0, 2);
		}
		else if (key_hit(KEY_UP))
		{
			VBlankIntrWait();
			playSound(9);
			selection -= 1;
			selection = CLAMP(selection, 0, 2);
		}

		fillArea(50, 86, 70, 116, 16);

		writeLine(">", 1, 27, 88 + selection * 16, 15, true);

		vid_flip();
		VBlankIntrWait();
	}
}

bool renderPauseMenu(int *map, char *visited, int playerX, int playerY)
{
	pauseTimer();
	drawHud();
	updateAmmo();

	VBlankIntrWait();
	playSound(9);

	while (1)
	{
		renderPause1st(map, visited, playerX, playerY);

		while (1)
		{
			key_poll();

			if (key_hit(KEY_RIGHT) || key_hit(KEY_LEFT) || key_hit(KEY_R) || key_hit(KEY_L))
			{
				VBlankIntrWait();
				playSound(9);
				int selection = renderPause2nd();
				if (selection == 1)
				{
					VBlankIntrWait();
					playSound(9);

					resumeTimer();
					return true;
				}
				if (selection == -1)
				{
					VBlankIntrWait();
					playSound(9);
					return false;
				}
				break;
			}

			if (key_hit(KEY_START))
			{
				VBlankIntrWait();
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

	fillArea(8, 10, 230, 115, 16);

	writeLine("LEVEL DONE", 10, 15, 20, 15, true);

	const char s1 = (time % 60 % 10) + '0';
	const char s0 = (time % 60 / 10) + '0';
	const char m1 = (time / 60) + '0';
	const char m0 = (time / 60 / 10) + '0';
	const char h1 = (time / 3600) + '0';
	const char h0 = (time / 3600 / 10) + '0';

	char timeLabel[8] = {h0, h1, ':', m0, m1, ':', s0, s1};
	writeLine("TIME", 4, 42, 40, 15, true);
	writeLine(timeLabel, 8, 25, 56, 15, true);

	writeLine("PRESS A", 7, 28, 80, 15, true);
	writeLine("TO CONTINUE", 11, 10, 96, 15, true);

	vid_flip();
	VBlankIntrWait();

	while (1)
	{
		key_poll();
		if (key_hit(KEY_A))
		{
			VBlankIntrWait();
			playSound(9);
			break;
		}
		VBlankIntrWait();
	}
}