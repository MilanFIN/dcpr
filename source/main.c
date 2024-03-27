
#include <tonc.h>
// #include "textures.h"
#include "structs.h"
#include "dungeon.h"
#include "render.h"
#include "menu.h"
#include "utils.h"
#include "raycaster.h"
#include "audio.h"
#include "timer.h"
#include "entities.h"
#include "hud.h"
#include "toplayer.h"

#define TILESIZE 1
const FIXED FIXEDTILESIZE = TILESIZE * 256;

volatile unsigned short *palette = (volatile unsigned short *)0x5000000;
int nextPaletteIndex = 0;

int utilityCounter = 0;
const int UTILITYRESET = 10;

/// @brief write a color value to next available slot in palette memory
/// @param c Libtonc builtin 15 bit rgb color value
void loadColorToPalette(COLOR c)
{
	palette[nextPaletteIndex] = c;
	nextPaletteIndex++;
}

// load 8 shades of a color to palette
void initShadeOfColor(float r, float g, float b)
{
	for (int i = 0; i < 8; i++)
	{
		loadColorToPalette(RGB15(4 * i * r, 4 * i * g, 4 * i * b));
	}
}

/// @brief initialize all colors into palette memory
void initPalette()
{
	initShadeOfColor(1, 1, 1);
	initShadeOfColor(1, 0, 0);
	initShadeOfColor(0, 1, 0);
	initShadeOfColor(0, 0, 1);
	initShadeOfColor(1, 1, 0);
	initShadeOfColor(1, 0, 1);
	initShadeOfColor(0, 1, 1);
	initShadeOfColor(1, 0.5, 0);
	initShadeOfColor(1, 0, 0.5);
	initShadeOfColor(0, 1, 0.5);
	initShadeOfColor(0, 0.5, 1);
	initShadeOfColor(0.5, 0, 1);
	initShadeOfColor(0.5, 1, 0);
	initShadeOfColor(1, 0.5, 0.5);
	initShadeOfColor(0.5, 1, 0.5);
	initShadeOfColor(0.5, 0.5, 1);
	initShadeOfColor(1, 0.6, 0.3);
	initShadeOfColor(0.7, 0.7, 1);
	initShadeOfColor(1, 0.8, 0.5);

	initShadeOfColor(1, 0.9, 0.7);
}

/// @brief set player direction vectors based on player view angle
void updateDirection()
{

	FIXED viewPlaneMultiplier = 220; //84 ~30, 168 ~60, 210~70

	const FIXED PI = int2fx(0x10000 >> 1);
	FIXED luAngle = fxmul(PI, fxdiv(player.direction, int2fx(360))) >> 7;

	FIXED cosine = lu_cos(luAngle);
	FIXED sine = lu_sin(luAngle);

	dirX = (fxdiv(cosine, int2fx(16)));
	dirY = (-fxdiv(sine, int2fx(16)));

	planeY = (fxdiv(fxmul(cosine, viewPlaneMultiplier), int2fx(16)));
	planeX = (fxdiv(fxmul(sine, viewPlaneMultiplier), int2fx(16)));
}

/// @brief move player while taking into account collision
/// @param type direction index
void move(int type)
{
	const FIXED SPEED = player.speed;
	FIXED moveX = 0;
	FIXED moveY = 0;

	if (type == 0)
	{ // left1
		moveX = fxmul(dirY, SPEED);
		moveY = -fxmul(dirX, SPEED);
	}
	else if (type == 1)
	{ // right
		moveX = -fxmul(dirY, SPEED);
		moveY = fxmul(dirX, SPEED);
	}
	else if (type == 2)
	{ // forward
		moveX = fxmul(dirX, SPEED);
		moveY = fxmul(dirY, SPEED);
	}
	else
	{ // back
		moveX = -fxmul(dirX, SPEED);
		moveY = -fxmul(dirY, SPEED);
	}

	if (!collisionCheck(fxadd(player.x, fxadd(moveX, moveX)), player.y))
	{
		player.x = fxadd(player.x, moveX);
	}
	if (!collisionCheck(player.x, fxadd(player.y, fxadd(moveY, moveY))))
	{
		player.y = fxadd(player.y, moveY);
	}
	moveHands();
}

/// @brief check if a tile value is 0 (open)
/// @param x tile index
/// @param y tile index
/// @return true if tile is open, or out of bounds
int openTile(int x, int y)
{
	if (x < 0 || x >= MAPSIZE)
	{
		return 0;
	}
	if (y < 0 || y >= MAPSIZE)
	{
		return 0;
	}
	if (MAP[MAPSIZE * y + x] == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/// @brief set position of key entity in relation to the door "wall" in the map
/// @param doorX door position in tile space (0 to mapsize)
/// @param doorY
void setKeyPosition(int doorX, int doorY)
{
	int fractionOfMap = 1;
	while (1)
	{
		// get all positions that are at least 1/3 MAPSIZE away from the door
		int counter = 0;
		for (int y = 0; y < MAPSIZE; y++)
		{
			for (int x = 0; x < MAPSIZE; x++)
			{
				if (openTile(x, y))
				{
					if (intAbs(x - doorX) + intAbs(y - doorY) > MAPSIZE / fractionOfMap)
					{
						UTILITYMAPDATA[counter] = MAPSIZE * y + x;
						counter++;
					}
				}
			}
		}

		if (counter != 0)
		{
			shuffleArray(UTILITYMAPDATA, counter);
			counter--;

			int keyY = UTILITYMAPDATA[counter] / MAPSIZE;
			int keyX = UTILITYMAPDATA[counter] % MAPSIZE;

			initKey(keyX, keyY);

			break;
		}
		else
		{
			fractionOfMap += 1;
		}
	}
}

/// @brief check if at least one of the 4 adjacent tiles is open (val == 0),
///			then return position to such open tile and direction away from the original tile
/// @param oX position of the tile in tile space (0 to mapsize)
/// @param oY position of the tile
/// @param x saves result of the coordinate of the open position next to the specified tile
/// @param y -||-
/// @param direction direction that is away from the original tile
void getOpenAdjacentTile(int oX, int oY, FIXED *x, FIXED *y, FIXED *direction)
{

	if (openTile(oX + 1, oY))
	{
		*x = int2fx(oX + 1) + 128;
		*y = int2fx(oY) + 128;
		*direction = int2fx(0);
	}
	else if (openTile(oX - 1, oY))
	{
		*x = int2fx(oX - 1) + 128;
		*y = int2fx(oY) + 128;
		*direction = int2fx(180);
	}
	else if (openTile(oX, oY + 1))
	{
		*x = int2fx(oX) + 128;
		*y = int2fx(oY + 1) + 128;
		*direction = int2fx(270);
	}
	else if (openTile(oX, oY - 1))
	{
		*x = int2fx(oX) + 128;
		*y = int2fx(oY - 1) + 128;
		*direction = int2fx(90);
	}
}

/// @brief take a map that only contains walls and open spaces and add items & door to that map
void populateMap()
{

	int counter = 0;
	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			// check for a valid door position, which would be one that
			// is blocked, but still has one adjacent clear tile
			if (MAP[MAPSIZE * y + x])
			{
				if (openTile(x + 1, y) + openTile(x - 1, y) + openTile(x, y + 1) + openTile(x, y - 1) == 1)
				{
					UTILITYMAPDATA[counter] = MAPSIZE * y + x;
					counter++;
				}
			}
		}
	}

	int doorPosition = qran_range(0, counter);
	// set to door sprite
	MAP[UTILITYMAPDATA[doorPosition]] = 5;

	int doorX = UTILITYMAPDATA[doorPosition] % MAPSIZE;
	int doorY = UTILITYMAPDATA[doorPosition] / MAPSIZE;

	// set player position to the free tile next to the door
	getOpenAdjacentTile(doorX, doorY, &player.x, &player.y, &player.direction);

	// set key position, it should be relatively far to allow for more walking around
	setKeyPosition(doorX, doorY);

	// get all open positions on map for spawning other stuff
	counter = 0;
	int playerPosition = MAPSIZE * fx2int(player.y) + fx2int(player.x);
	for (int y = 0; y < MAPSIZE; y++)
	{
		for (int x = 0; x < MAPSIZE; x++)
		{
			if (openTile(x, y))
			{
				if (MAP[MAPSIZE * y + x] == 0 && MAPSIZE * y + x != playerPosition)
				{
					UTILITYMAPDATA[counter] = MAPSIZE * y + x;
					counter++;
				}
			}
		}
	}

	shuffleArray(UTILITYMAPDATA, counter);
	counter--;

	// spawn a bunch of items (hp, weapon upgrades etc.)
	for (int j = 0; j < mapSize / 4; j++)
	{
		counter--;
		if (counter < 0)
		{
			break;
		}
		int x = UTILITYMAPDATA[counter] % MAPSIZE;
		int y = UTILITYMAPDATA[counter] / MAPSIZE;
		int type = j < 2 ? 0 : qran_range(1, 3);
		initPickup(type, j + 2, x, y);
	}
}

/// @brief generate a new level and reset player state
void initLevel()
{

	player.hp = player.maxHp;
	player.ammo = player.maxAmmo;
	player.speed = float2fx(0.15);
	player.hasKey = false;
	player.gunLevel = 1;
	player.maxGunLevel = 3;
	player.damage = 1;
	player.overdrive = 0;

	player.ammoRecoveryRate = difficulty > 0 ? 1 : 2;

	updateHud = 2;

	// player.x = int2fx(5);//96;//2*64;//
	// player.y = int2fx(5);//224;//2*64;//

	// player.direction = int2fx(0);//int2fx(45);

	getDungeon(MAP, mapSize);

	for (int i = 0; i < MAPSIZE * MAPSIZE; i++)
	{
		VISITEDLOCATIONS[i] = 0;
	}

	initEntities();

	populateMap();

	startTimer1s();
}

void updatePlayerVisited()
{
	if (player.previousX != fx2int(player.x) || player.previousY != fx2int(player.y))
	{
		if (VISITEDLOCATIONS[fx2int(player.y) * MAPSIZE + fx2int(player.x)] != 2)
		{
			int x = fx2int(player.x);
			int y = fx2int(player.y);
			// int MINIMAPVISIBLERADIUS = 3;

			for (int i = x - MINIMAPVISIBLERADIUS; i < x + MINIMAPVISIBLERADIUS + 1; i++)
			{
				for (int j = y - MINIMAPVISIBLERADIUS; j < y + MINIMAPVISIBLERADIUS + 1; j++)
				{
					if (i >= 0 && i < mapSize && j >= 0 && j < mapSize)
					{
						if ((i - x) * (i - x) + (j - y) * (j - y) <= MINIMAPVISIBLERADIUS * MINIMAPVISIBLERADIUS)
						{
							VISITEDLOCATIONS[MAPSIZE * j + i] = 1;
						}
					}
				}
			}
			VISITEDLOCATIONS[MAPSIZE * y + x] = 2;
		}
	}
	player.previousX = fx2int(player.x);
	player.previousY = fx2int(player.y);
}

void syncVideoBuffers()
{
	// clean up difference between video buffers due to movement
	castRays();
	drawEntities();
	drawHud();
	drawHands();
	updateAmmo();
	vid_flip();
	castRays();
	drawEntities();
	drawHud();
	drawHands();
	updateAmmo();
}

/// @brief main game logic
void mainGameLoop()
{
	while (1)
	{

		key_poll();
		if (key_is_down(KEY_L))
		{
			move(0);
		}
		if (key_is_down(KEY_R))
		{
			move(1);
		}
		if (key_is_down(KEY_UP))
		{
			move(2);
		}
		else if (key_is_down(KEY_DOWN))
		{
			move(3);
		}
		if (key_is_down(KEY_RIGHT))
		{
			player.direction -= int2fx(5);
		}
		if (key_is_down(KEY_LEFT))
		{
			player.direction += int2fx(5);
		}
		if (fx2int(player.direction) >= 360)
		{
			player.direction = fxsub(player.direction, int2fx(360));
		}
		if (fx2int(player.direction) < 0)
		{
			player.direction = fxadd(player.direction, int2fx(360));
		}

		if (key_hit(KEY_A))
		{
			if (castForward(dirX, dirY))
			{
				pauseTimer();
				syncVideoBuffers();
				VBlankIntrWait();
				playSound(18);
				endAnimation(16);
				renderLevelDone();
				break;
			}
		}
		if (key_hit(KEY_B))
		{
			fire(dirX, dirY);
		}
		if (key_hit(KEY_START))
		{
			bool resume = renderPauseMenu(MAP, VISITEDLOCATIONS, fx2int(player.x), fx2int(player.y));
			if (!resume)
			{
				break;
			}

			updateHud = 2;
			updateCompass = 2;
		}

		updateDirection();

		castRays();
		moveEntities();
		drawEntities();
		drawHands();
		checkEntityCollisions();
		updatePlayerVisited();

		if (updateHud)
		{
			drawHud();
			updateHud--;
		}

		checkCompass();
		updateAmmo();

		if (player.hp <= 0)
		{
			VBlankIntrWait();
			playSound(11);
			syncVideoBuffers();
			endAnimation(14);
			break;
		}

		if (utilityCounter == 0)
		{
			pruneFarAwayEnemies();
		}
		if (utilityCounter % 2 == 0)
		{
			sortEntities();
		}
		if (utilityCounter == 5)
		{
			refillEnemies();
		}
		if (player.overdrive > 0)
		{
			player.overdrive--;
		}
		utilityCounter--;
		if (utilityCounter < 0)
		{
			utilityCounter = UTILITYRESET;
		}
		if (notificationCounter > 0)
		{
			writeLine(notification, notificationLength, 118 - 9 * notificationLength, 0, 15, true);
			notificationCounter--;
		}

		vid_flip();
		// VBlankIntrWait();
	}
}

int main()
{
	REG_DISPCNT = DCNT_MODE4 | DCNT_BG2;

	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	initAudio();
	sqran(1);
	initPalette();
	renderStart();

	mapSize = 50; // 50 default, 60 or 70 highest tested

	while (1)
	{
		renderMainMenu();
		initLevel();
		mainGameLoop();
	}
}
