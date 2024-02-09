
#include <tonc.h>
// #include "textures.h"
#include "structs.h"
#include "dungeon.h"
#include "render.h"
#include "menu.h"
#include "utils.h"

#define TILESIZE 1
#define FOV 60
#define MAXENTITYCOUNT 20

const float PI = 3.1415;

EWRAM_DATA char MAP[MAPSIZE * MAPSIZE] = {0};

/*
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 4, 1, 1, 1
	};
	*/
EWRAM_DATA int UTILITYMAPDATA[MAPSIZE * MAPSIZE] = {0};
EWRAM_DATA char VISITEDLOCATIONS[MAPSIZE * MAPSIZE] = {0};


const int CASTEDRAYS = SCREENWIDTH / 2;
const int HALFFOV = FOV / 2;
const FIXED FIXEDTILESIZE = TILESIZE * 256; // equal to int2fx(TILESIZE);
const int HALFSCREENPOINT = SCREENHEIGHT / 2;
const int HUDHEIGHT = 160 - SCREENHEIGHT;

const int PROJECTILETEXTURES[3] = {6, 9, 10};
const int ENEMYTEXTURES[4] = {7, 8, 13, 14};


FIXED dirX, dirY;
FIXED planeX, planeY;
int updateHud = 2;

EWRAM_DATA struct Entity entities[MAXENTITYCOUNT];
struct Player player;

int entityOrder[MAXENTITYCOUNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
FIXED zBuffer[SCREENWIDTH / 2] = {0};

volatile unsigned short *palette = (volatile unsigned short *)0x5000000;
int nextPaletteIndex = 0;

int utilityCounter = 0;
const int UTILITYRESET = 10;

const int goalEnemyCount = 3;
const int pruneEnemyDistance = 12;

char notification[5] = "ABCDE";
size_t notificationLength = 5;
int notificationCounter = 0;
const int notificationDuration = 20;

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

/// @brief initialize lookup table for direction vectors for camera
void initCameraXLu()
{
	for (int i = 0; i < CASTEDRAYS; i++)
	{
		CAMERAX_LU[i] = fxsub(fxdiv(int2fx(2 * i), int2fx(CASTEDRAYS)), int2fx(1));
	}
}

/// @brief initialize a lookup table for how wide each texture column is based on texture size
void initTextureStepLu()
{
	for (int i = 0; i < SCREENHEIGHT; i++)
	{
		TEXTURESTEP_LU[i] = fxdiv(int2fx(TEXTURESIZE), int2fx(i));
	}
}

/// @brief initialize "key" entity position, key is always first in entity array
/// @param x tile position (0 to mapsize)
/// @param y -||-
void initKey(int x, int y)
{
	entities[0].active = true;
	entities[0].x = int2fx(x) + 128;
	entities[0].y = int2fx(y) + 128;
	entities[0].texture = 3;
	entities[0].type = 1;
	entities[0].scale = 128;
	entities[0].moving = false;
	entities[0].yOffset = 128;
	entities[0].hit = 0;
	copyText(entities[0].notification, "KEY", 3);
	entities[0].notificationLength = 3;
}

/// @brief initialize an enemy entity
/// @param id position of the entity in entity array
/// @param x tile position (0 to mapsize)
/// @param y -||-
void initEnemy(int id, int x, int y)
{

	int level = qran_range(0, 8);
	int enemyType = qran_range(0, 4);


	entities[id].active = true;
	entities[id].x = int2fx(x) + 128;
	entities[id].y = int2fx(y) + 128;
	entities[id].texture = ENEMYTEXTURES[enemyType];
	entities[id].type = 3;
	entities[id].scale = 92 + (level * 32);
	entities[id].moving = true;
	entities[id].yOffset = 192 - entities[id].scale * 2 / 3;
	entities[id].speed = 4;
	entities[id].attackDelay = 20;
	entities[id].attackFrequency = 20;
	entities[id].damage = 10 + 2 * level;
	entities[id].hp = level;
	entities[id].hit = 0;
}

/// @brief initialize an item pickup
/// @param id position in entity array
/// @param x tile position (0 to mapsize)
/// @param y
void initPickup(int id, int x, int y)
{
	int pickupType = qran_range(0, 2);

	if (pickupType == 0)
	{ // healthpack
		entities[id].active = true;
		entities[id].x = int2fx(x) + 128;
		entities[id].y = int2fx(y) + 128;
		entities[id].texture = 12;
		entities[id].type = 4;
		entities[id].scale = 128;
		entities[id].moving = false;
		entities[id].yOffset = 128;
		entities[id].hit = 0;
		entities[id].damage = 50;
		copyText(entities[id].notification, "HP:", 3);
		entities[id].notificationLength = 3;
	}
	else if (pickupType == 1)
	{ // gun level up
		entities[id].active = true;
		entities[id].x = int2fx(x) + 128;
		entities[id].y = int2fx(y) + 128;
		entities[id].texture = 11;
		entities[id].type = 5;
		entities[id].scale = 128;
		entities[id].moving = false;
		entities[id].yOffset = 128;
		entities[id].hit = 0;
		copyText(entities[id].notification, "SPELL", 5);
		entities[id].notificationLength = 5;
	}
}

/// @brief initialize entity array with unused values
void initEntities()
{
	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		entities[i].active = false;
	}
}

/// @brief draw player hud and contents
void drawHud()
{
	// background
	fillArea(0, 160 - HUDHEIGHT, 2 * CASTEDRAYS, 160, 29);
	// key icon
	if (player.hasKey)
	{
		drawFlat(TEXTURES, 3, 10, 160 - HUDHEIGHT - 10, 16, 16, 0, TEXTURESIZE);
	}
	// health bar
	fillArea(120, 160 - HUDHEIGHT + 5, 120 + player.hp / 2, 160 - 5, 15);
}

/// @brief draw a single pixel column on screen, inc. roof, wall and floor
/// @param i column (0-120)
/// @param distance distance to the wall (determines wall height)
/// @param type wall texture (indexed from 1)
/// @param vertical true if wall is vertical in 2d map space
/// @param textureColumn which column of the texture is to be read
void drawWall(int i, FIXED distance, int type, int vertical, int textureColumn)
{

	int wallHeight = fx2int(fxdiv(int2fx(SCREENHEIGHT), distance));
	wallHeight = CLAMP(wallHeight, 1, SCREENHEIGHT);
	int halfHeight = (wallHeight >> 1);
	int color = 1;

	// roof
	m4_dual_vline(i, 0, HALFSCREENPOINT - halfHeight, color);

	color = 131; // 59;//4;
	const FIXED yStep = TEXTURESTEP_LU[wallHeight];

	// the actual wall
	m4_textured_dual_line(TEXTURES, i, HALFSCREENPOINT - halfHeight, HALFSCREENPOINT + halfHeight, type, vertical, textureColumn, yStep, TEXTURESIZE);
	// floor
	m4_dual_vline(i, HALFSCREENPOINT + halfHeight, SCREENHEIGHT, color);
}

void drawWithoutWall(int i)
{
	int color = 1;
	// roof
	m4_dual_vline(i, 0, HALFSCREENPOINT, color);
	color = 131; // 59;//4;
	// floor
	m4_dual_vline(i, HALFSCREENPOINT, SCREENHEIGHT, color);
}

/// @brief cast a ray forward in 2d plane, and check if specified wall is hit
/// @param targetType texture of target wall
/// @return true, if specified target was hit
int castRay(int targetType)
{
	const FIXED cameraX = CAMERAX_LU[CASTEDRAYS / 2]; // x-coordinate in camera space
	const FIXED rayDirX = fxadd(dirX, fxmul(planeX, cameraX));
	const FIXED rayDirY = fxadd(dirY, fxmul(planeY, cameraX));

	FIXED mapX = int2fx(fx2int(player.x));
	FIXED mapY = int2fx(fx2int(player.y));

	FIXED sideDistX;
	FIXED sideDistY;

	FIXED deltaDistX;
	FIXED deltaDistY;
	if (rayDirX == 0)
	{
		deltaDistX = int2fx(1024);
	}
	else
	{
		deltaDistX = fixedAbs(fxdiv(int2fx(1), rayDirX));
	}

	if (rayDirY == 0)
	{
		deltaDistY = int2fx(1024);
	}
	else
	{
		deltaDistY = fixedAbs(fxdiv(int2fx(1), rayDirY));
	}

	FIXED stepX;
	FIXED stepY;

	int side;

	if (rayDirX < 0)
	{
		stepX = int2fx(-1);
		sideDistX = fxmul(fxsub(player.x, mapX), deltaDistX);
	}
	else
	{
		stepX = int2fx(1);
		sideDistX = fxmul(fxsub(fxadd(mapX, int2fx(1.0)), player.x), deltaDistX);
	}
	if (rayDirY < 0)
	{
		stepY = int2fx(-1);
		sideDistY = fxmul(fxsub(player.y, mapY), deltaDistY);
	}
	else
	{
		stepY = int2fx(1);
		sideDistY = fxmul(fxsub(fxadd(mapY, int2fx(1.0)), player.y), deltaDistY);
	}

	int hit = 0;
	for (int j = 0; j < 100; j++)
	{
		if (sideDistX < sideDistY)
		{
			sideDistX = fxadd(sideDistX, deltaDistX);
			mapX = fxadd(mapX, stepX);
			side = 0;
		}
		else
		{
			sideDistY = fxadd(sideDistY, deltaDistY);
			mapY = fxadd(mapY, stepY);
			side = 1;
		}

		int currentXCell = fx2int(mapX);
		int currentYCell = fx2int(mapY);
		if (MAP[currentYCell * MAPSIZE + currentXCell] > 0)
		{
			if (MAP[currentYCell * MAPSIZE + currentXCell] == targetType)
			{
				hit = 1;
			}
			break;
		}
	}

	FIXED perpWallDistance = -256;

	if (hit)
	{
		if (side == 0)
		{
			perpWallDistance = fxsub(sideDistX, deltaDistX);
		}
		// vertical
		else
		{
			perpWallDistance = fxsub(sideDistY, deltaDistY);
		}
	}

	return fx2int(perpWallDistance);
}

/// @brief main raycast method. Loops through the columns of screen (120, when at half resolution)
void castRays()
{
	for (int i = 0; i < CASTEDRAYS; i++)
	{

		const FIXED cameraX = CAMERAX_LU[i]; // x-coordinate in camera space
		const FIXED rayDirX = fxadd(dirX, fxmul(planeX, cameraX));
		const FIXED rayDirY = fxadd(dirY, fxmul(planeY, cameraX));

		FIXED mapX = int2fx(fx2int(player.x));
		FIXED mapY = int2fx(fx2int(player.y));

		FIXED sideDistX;
		FIXED sideDistY;

		FIXED deltaDistX;
		FIXED deltaDistY;
		if (rayDirX == 0)
		{
			deltaDistX = int2fx(1024);
		}
		else
		{
			deltaDistX = fixedAbs(fxdiv(int2fx(1), rayDirX));
		}

		if (rayDirY == 0)
		{
			deltaDistY = int2fx(1024);
		}
		else
		{
			deltaDistY = fixedAbs(fxdiv(int2fx(1), rayDirY));
		}

		FIXED stepX;
		FIXED stepY;

		int side;

		if (rayDirX < 0)
		{
			stepX = int2fx(-1);
			sideDistX = fxmul(fxsub(player.x, mapX), deltaDistX);
		}
		else
		{
			stepX = int2fx(1);
			sideDistX = fxmul(fxsub(fxadd(mapX, int2fx(1.0)), player.x), deltaDistX);
		}
		if (rayDirY < 0)
		{
			stepY = int2fx(-1);
			sideDistY = fxmul(fxsub(player.y, mapY), deltaDistY);
		}
		else
		{
			stepY = int2fx(1);
			sideDistY = fxmul(fxsub(fxadd(mapY, int2fx(1.0)), player.y), deltaDistY);
		}

		int hit = 0;
		int xCell;
		int yCell;
		for (int j = 0; j < 100; j++)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX = fxadd(sideDistX, deltaDistX);
				mapX = fxadd(mapX, stepX);
				side = 0;
			}
			else
			{
				sideDistY = fxadd(sideDistY, deltaDistY);
				mapY = fxadd(mapY, stepY);
				side = 1;
			}

			int currentXCell = fx2int(mapX);
			int currentYCell = fx2int(mapY);
			if (MAP[currentYCell * MAPSIZE + currentXCell] > 0)
			{
				xCell = currentXCell;
				yCell = currentYCell;
				hit = 1;
				break;
			}
		}

		FIXED perpWallDistance;
		int textureColumn;
		FIXED wallPos;

		if (hit)
		{
			// horizontal
			if (side == 0)
			{
				perpWallDistance = fxsub(sideDistX, deltaDistX);
				wallPos = fxadd(player.y, fxmul(perpWallDistance, rayDirY));
			}
			// vertical
			else
			{
				perpWallDistance = fxsub(sideDistY, deltaDistY);
				wallPos = fxadd(player.x, fxmul(perpWallDistance, rayDirX));
			}

			wallPos = fxsub(wallPos, int2fx(fx2int(wallPos)));
			textureColumn = fx2int(fxmul(wallPos, int2fx(TEXTURESIZE)));

			if (side == 0 && rayDirX < 0)
				textureColumn = TEXTURESIZE - textureColumn - 1;
			if (side == 1 && rayDirY > 0)
				textureColumn = TEXTURESIZE - textureColumn - 1;

			zBuffer[i] = perpWallDistance;
			drawWall(2 * i, perpWallDistance, MAP[yCell * MAPSIZE + xCell], side, textureColumn);
		}
		else
		{
			drawWithoutWall(2 * i);
		}
	}
}

/// @brief create a new projectile entity with speed and direction
void fire()
{
	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		if (entities[i].active)
		{
			continue;
		}

		entities[i].x = player.x;
		entities[i].y = player.y;
		entities[i].texture = PROJECTILETEXTURES[player.gunLevel - 1];
		entities[i].type = 2;
		entities[i].active = true;
		entities[i].scale = 64;
		entities[i].xDir = dirX;
		entities[i].yDir = dirY;
		entities[i].moving = true;
		entities[i].yOffset = 256;
		entities[i].hit = 0;

		break;
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

/// @brief move projectile and enemy entities
void moveEntities()
{
	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		if (!entities[i].active || !entities[i].moving)
		{
			continue;
		}
		if (entities[i].type == 2)
		{ // projectile
			entities[i].x = fxadd(entities[i].x, entities[i].xDir);
			entities[i].y = fxadd(entities[i].y, entities[i].yDir);
		}
		if (entities[i].type == 3)
		{

			if (entities[i].distance < 3000 && entities[i].distance > 128)
			{
				if (entities[i].x < player.x)
				{
					entities[i].xDir = entities[i].speed;
				}
				else
				{
					entities[i].xDir = -entities[i].speed;
				}
				if (entities[i].y < player.y)
				{
					entities[i].yDir = entities[i].speed;
				}
				else
				{
					entities[i].yDir = -entities[i].speed;
				}
				FIXED newX = fxadd(entities[i].x, entities[i].xDir);
				FIXED newY = fxadd(entities[i].y, entities[i].yDir);

				if (!collisionCheck(newX, entities[i].y))
				{
					entities[i].x = newX;
				}

				if (!collisionCheck(entities[i].x, newY))
				{
					entities[i].y = newY;
				}
			}
		}
	}
}

/// @brief helper for quicksort
/// @param v1
/// @param v2
/// @return
int compareDistances(const void *v1, const void *v2)
{
	const struct Entity *u1 = v1;
	const struct Entity *u2 = v2;
	return u1->distance < u2->distance;
}

/// @brief helper for quicksort, swap places of two items
/// @param a
/// @param b
void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

/// @brief helper for quicksort
/// @param array
/// @param low
/// @param high
/// @return
int partition(int array[], int low, int high)
{

	// select the rightmost element as pivot
	int pivot = entities[array[high]].distance;

	// pointer for greater element
	int i = (low - 1);

	// traverse each element of the array
	// compare them with the pivot
	for (int j = low; j < high; j++)
	{
		if (entities[array[j]].distance >= pivot)
		{

			// if element smaller than pivot is found
			// swap it with the greater element pointed by i
			i++;

			// swap element at i with element at j
			swap(&array[i], &array[j]);
		}
	}

	// swap the pivot element with the greater element at i
	swap(&array[i + 1], &array[high]);

	// return the partition point
	return (i + 1);
}

/// @brief main quicksort function, used to order entities based on distance to player
/// @param array
/// @param low
/// @param high
void quickSort(int array[], int low, int high)
{
	if (low < high)
	{

		// find the pivot element such that
		// elements smaller than pivot are on left of pivot
		// elements greater than pivot are on right of pivot
		int pi = partition(array, low, high);

		// recursive call on the left of pivot
		quickSort(array, low, pi - 1);

		// recursive call on the right of pivot
		quickSort(array, pi + 1, high);
	}
}

/// @brief sort entities based on distance to player
void sortEntities()
{
	quickSort(entityOrder, 0, MAXENTITYCOUNT - 1);
}

/// @brief draw all visible entities on screen, if not hidden by walls
void drawEntities()
{

	// update distances to player
	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		if (!entities[i].active)
		{
			entities[i].distance = int2fx(4096);
		}
		else
		{
			entities[i].distance = fxmul(fxsub(player.x, entities[i].x), fxsub(player.x, entities[i].x)) + fxmul(fxsub(player.y, entities[i].y), fxsub(player.y, entities[i].y));
		}
	}

	// sortEntities();

	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		if (!entities[entityOrder[i]].active || entities[entityOrder[i]].distance < 64)
		{
			continue;
		}
		if (entities[entityOrder[i]].type == 1 && entities[entityOrder[i]].distance < 150)
		{
			continue;
		}
		FIXED entityX = fxsub(entities[entityOrder[i]].x, player.x);
		FIXED entityY = fxsub(entities[entityOrder[i]].y, player.y);

		FIXED invDet = fxdiv(int2fx(1), fxsub(fxmul(planeX, dirY), fxmul(dirX, planeY)));
		FIXED transformX = fxmul(invDet, fxsub(fxmul(dirY, entityX), fxmul(dirX, entityY)));
		FIXED transformY = fxmul(invDet, fxadd(fxmul(-planeY, entityX), fxmul(planeX, entityY)));

		if (transformX == 0)
		{
			transformX = 16;
		}
		if (transformY == 0)
		{
			transformY = 16;
		}

		int spriteScreenX = fx2int(fxmul(int2fx(SCREENWIDTH >> 1), (fxadd(int2fx(1), fxdiv(transformX, transformY)))));

		// calculate height of the sprite on screen
		int spriteHeight = fixedAbs(fx2int(fxdiv(fxmul(int2fx(SCREENHEIGHT), entities[entityOrder[i]].scale), (transformY)))); // using 'transformY' instead of the real distance prevents fisheye
		int offsetY = fx2int(fxmul(int2fx(spriteHeight), entities[entityOrder[i]].yOffset));
		// calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -spriteHeight / 2 + SCREENHEIGHT / 2 + offsetY;
		if (drawStartY < 0)
			drawStartY = 0;
		int drawEndY = spriteHeight / 2 + SCREENHEIGHT / 2 + offsetY;
		if (drawEndY >= SCREENHEIGHT)
			drawEndY = SCREENHEIGHT - 1;

		// calculate width of the sprite
		int spriteWidth = spriteHeight;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		drawStartX /= 2;
		drawEndX /= 2;

		FIXED horizontalTexFrag = fxdiv(int2fx(2 * TEXTURESIZE), int2fx(spriteWidth));
		// keeps track of which column of the texture should be drawn, (FIXED point fractional)
		// starting from zero leads to artifacting on the very first vertical stripe
		// instead first column set to 32/256 => 0.125
		FIXED hTexPos = 32;
		if (transformY > 0)
		{

			int texture = entities[entityOrder[i]].texture;
			int hit = entities[entityOrder[i]].hit;

			for (int stripe = drawStartX; stripe < drawEndX; stripe++)
			{
				if (stripe >= 0 && stripe < SCREENWIDTH / 2)
				{
					if (transformY < zBuffer[stripe])
					{
						int texX = fx2int(hTexPos);
						const FIXED yStep = TEXTURESTEP_LU[drawEndY - drawStartY];

						if (!hit)
						{
							m4_sprite_textured_dual_line(TEXTURES, 2 * stripe, drawStartY, drawEndY, texture, texX, yStep, TEXTURESIZE);
						}
						else
						{
							int color = 11;
							m4_sprite_color_textured_dual_line(TEXTURES, 2 * stripe, drawStartY, drawEndY, texture, texX, color, yStep, TEXTURESIZE);
						}
					}
				}
				else if (stripe > SCREENWIDTH / 2)
				{
					break;
				}
				hTexPos = fxadd(hTexPos, horizontalTexFrag);
			}
		}
		entities[entityOrder[i]].hit = 0;
	}
}

/// @brief randomize order of an array in place
/// @param array array pointer
/// @param size size of array
void shuffleArray(int *array, int size)
{
	for (int i = 0; i < size; i++)
	{
		int j = qran_range(0, size);
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

int simpleDistance(int x1, int y1, int x2, int y2)
{
	return intAbs(x2 - x1) + intAbs(y2 - y1);
}

/// @brief disables an entity from being drawn
/// @param i entity array index
void removeEntity(int i)
{
	entities[i].active = false;
}

void refillEnemies()
{

	int count = 0;
	int firstFreeSlot = -1;
	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		if (entities[i].active && entities[i].type == 3)
		{
			count++;
		}
		if (firstFreeSlot < 0 && !entities[i].active)
		{
			firstFreeSlot = i;
		}
	}

	if (firstFreeSlot >= 0 && count < goalEnemyCount)
	{
		int counter = 0;
		int playerX = fx2int(player.x);
		int playerY = fx2int(player.y);

		for (int y = 0; y < MAPSIZE; y++)
		{
			for (int x = 0; x < MAPSIZE; x++)
			{
				if (MAP[y * MAPSIZE + x] == 0)
				{
					int distance = simpleDistance(x, y, playerX, playerY);
					if (distance > 5 && distance < 8)
					{
						UTILITYMAPDATA[counter] = y * MAPSIZE + x;
						counter++;
					}
				}
			}
		}

		// shuffleArray(UTILITYMAPDATA, counter);
		// TODO CONTINUE
		int position = qran_range(0, counter);
		int x = UTILITYMAPDATA[position] % MAPSIZE;
		int y = UTILITYMAPDATA[position] / MAPSIZE;

		initEnemy(firstFreeSlot, x, y);
	}
}

void pruneFarAwayEnemies()
{
	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		if (entities[i].active && entities[i].type == 3)
		{
			int distance = simpleDistance(fx2int(entities[i].x), fx2int(entities[i].y), fx2int(player.x), fx2int(player.y));
			if (distance >= pruneEnemyDistance)
			{
				removeEntity(i);
			}
		}
	}
}

void setNotification(int i)
{
	notificationCounter = notificationDuration;
	notificationLength = entities[i].notificationLength;
	copyText(notification, entities[i].notification, entities[i].notificationLength);
}

/// @brief check for collisions between entities and each other &/or player
void checkEntityCollisions()
{
	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		if (!entities[i].active)
		{
			continue;
		}

		int type = entities[i].type;
		// check if a key is near a player
		if (type == 1)
		{
			if (entities[i].distance < 150)
			{
				setNotification(i);
				removeEntity(i);
				player.hasKey = true;
				updateHud = 2;
			}
		}

		// check projectile collisions
		else if (type == 2)
		{

			int hit = 0;

			// with enemies
			for (int j = 0; j < MAXENTITYCOUNT; j++)
			{
				// ignore all entities, which aren't enemies
				if (!entities[j].active || entities[j].type != 3)
				{ //
					continue;
				}

				// check entity distances from each other
				FIXED distance = fxadd(fixedAbs(fxsub(entities[i].x, entities[j].x)), fixedAbs(fxsub(entities[i].y, entities[j].y)));
				if (distance < 180)
				{

					entities[j].hp -= player.gunLevel * player.damage;
					entities[j].hit = 1;
					hit = 1;

					if (entities[j].hp <= 0)
					{
						removeEntity(j);
					}
				}
			}
			if (hit)
			{
				removeEntity(i);
			}

			// with walls
			int xCell = fx2int(entities[i].x);
			int yCell = fx2int(entities[i].y);
			if (MAP[yCell * MAPSIZE + xCell] != 0)
			{
				removeEntity(i);
			}
		}

		// check enemy proximity with player
		else if (type == 3)
		{
			if (entities[i].distance < 150)
			{
				if (!entities[i].attackDelay)
				{
					player.hp -= entities[i].damage;
					updateHud = 2;
					entities[i].attackDelay = entities[i].attackFrequency;
				}
			}
			if (entities[i].attackDelay)
			{
				entities[i].attackDelay--;
			}
		}

		// check health pack proximity with player
		else if (type == 4)
		{
			if (entities[i].distance < 64)
			{
				if (player.hp < player.maxHp)
				{
					int heal = entities[i].damage;
					player.hp += heal;
					if (player.hp > player.maxHp)
					{
						player.hp = player.maxHp;
					}
					setNotification(i);
					removeEntity(i);
					updateHud = 2;
				}
			}
		}

		// check gun level up pickup proximity with player
		else if (type == 5)
		{
			if (entities[i].distance < 64)
			{
				if (player.gunLevel < player.maxGunLevel)
				{
					player.gunLevel++;
					setNotification(i);
					removeEntity(i);
					updateHud = 2;
				}
			}
		}
	}
}

/// @brief set player direction vectors based on player view angle
void updateDirection()
{

	FIXED viewPlaneMultiplier = 200; // 168;

	const FIXED PI2 = int2fx(0x10000 >> 1);
	FIXED luAngle = fxmul(PI2, fxdiv(player.direction, int2fx(360))) >> 7;

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

	// figure out a position for the door
	int UTILITYMAPDATA[MAPSIZE * MAPSIZE] = {0};

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
				if (!MAP[MAPSIZE * y + x] && MAPSIZE * y + x != playerPosition)
				{
					UTILITYMAPDATA[counter] = MAPSIZE * y + x;
					counter++;
				}
			}
		}
	}

	shuffleArray(UTILITYMAPDATA, counter);
	counter--;

	// spawn a bunch of enemies
	int i = 0;

	for (i = 0; i < 3; i++)
	{
		counter--;
		if (counter < 0)
		{
			break;
		}
		int enemyX = UTILITYMAPDATA[counter] % MAPSIZE;
		int enemyY = UTILITYMAPDATA[counter] / MAPSIZE;
		initEnemy(i + 1, enemyX, enemyY);
	}

	// todo: spawn a bunch of items (hp, weapon upgrades etc.)
	for (int j = i; j < 6; j++)
	{
		counter--;
		if (counter < 0)
		{
			break;
		}
		int x = UTILITYMAPDATA[counter] / MAPSIZE;
		int y = UTILITYMAPDATA[counter] % MAPSIZE;
		initPickup(j + 1, x, y);
	}
}

/// @brief generate a new level and reset player state
void initLevel()
{

	player.hp = 100;
	player.maxHp = 100;
	player.speed = float2fx(0.15);
	player.hasKey = false;
	player.gunLevel = 1;
	player.maxGunLevel = 3;
	player.damage = 1;

	updateHud = 2;

	// player.x = int2fx(5);//96;//2*64;//
	// player.y = int2fx(5);//224;//2*64;//

	// player.direction = int2fx(0);//int2fx(45);

	getDungeon(MAP, mapSize, &player.x, &player.y);

	for (int i = 0; i < MAPSIZE * MAPSIZE; i++) {
		VISITEDLOCATIONS[i] = 0;

	}

	initEntities();

	populateMap();
}

/// @brief check if player is looking at anything special (walls)
void castForward()
{
	// check if player has key and is looking at a door
	if (player.hasKey)
	{
		int distance = castRay(5);
		if (distance >= 0 && distance < 2)
		{
			initLevel();
		}
	}
}

void updatePlayerVisited() {
	VISITEDLOCATIONS[fx2int(player.y) * MAPSIZE + fx2int(player.x)] = 1;
}

/// @brief main game logic
void mainGameLoop()
{
	while (1)
	{

		key_poll();
		if (key_is_down(KEY_LEFT))
		{
			move(0);
		}
		else if (key_is_down(KEY_RIGHT))
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
		if (key_is_down(KEY_R))
		{
			player.direction -= int2fx(5);
		}
		else if (key_is_down(KEY_L))
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
			castForward();
		}
		if (key_hit(KEY_B))
		{
			fire();
		}
		if (key_hit(KEY_START))
		{
			castRays();
			drawEntities();
			vid_flip();
			castRays();
			drawEntities();
			vid_flip();

			renderPauseMenu(MAP, VISITEDLOCATIONS, fx2int(player.x), fx2int(player.y));

			updateHud = 2;
		}

		updateDirection();

		castRays();
		moveEntities();
		drawEntities();
		checkEntityCollisions();
			updatePlayerVisited();


		if (updateHud)
		{
			drawHud();
			updateHud--;
		}

		if (player.hp <= 0)
		{
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

		utilityCounter--;
		if (utilityCounter < 0)
		{
			utilityCounter = UTILITYRESET;
		}
		if (notificationCounter > 0)
		{
			writeLine(notification, notificationLength, 118 - 9 * notificationLength, 0, 15);
			notificationCounter--;
		}

		vid_flip();
		// VBlankIntrWait();
	}
}

int main()
{
	REG_DISPCNT = DCNT_MODE4 | DCNT_BG2;

	sqran(1);
	initCameraXLu();
	initTextureStepLu();
	initPalette();
	// renderStart();

	mapSize = 50; // 60 or 70 highest tested

	while (1)
	{

		// renderMenu();

		initLevel();

		mainGameLoop();
	}
}
