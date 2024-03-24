#include "entities.h"

const int ENEMYSIZES[6] = {1, 1, 2, 2, 3, 3};
const int ENEMYSPEEDS[6] = {13, 11, 10, 10, 10, 10};
const int ENEMYLEVELS[6] = {1, 1, 2, 2, 3, 3};
const int ENEMYTEXTURES[6] = {16, 15, 7, 8, 13, 14};
const int ENEMYOFFSETS[6] = {0,32,32,32,32,32};

// 7 eye
// 8 skull
//  13 wiz
// 14 orc
// 15 spider
// 16 bat

const int ENEMYTEXCOUNT = 6;

struct Entity entities[MAXENTITYCOUNT];
int entityOrder[MAXENTITYCOUNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

const int goalEnemyCount = 6;
const int easyGoalEnemyCount = 6;

const int PRUNEENEMYDISTANCE = 17;

/// @brief initialize an enemy entity
/// @param id position of the entity in entity array
/// @param x tile position (0 to mapsize)
/// @param y -||-
void initEnemy(int id, int x, int y)
{

	int enemyType = qran_range(0, ENEMYTEXCOUNT);
	int enemyLevel = ENEMYLEVELS[enemyType];

	const int speedModifier = difficulty > 0 ? 1 : 2;
	const int damageModifier = difficulty > 0 ? difficulty : -5;
	const int hpModifier = difficulty > 0 ? difficulty -1 : 0;


	entities[id].active = true;
	entities[id].x = int2fx(x) + 128;
	entities[id].y = int2fx(y) + 128;
	entities[id].texture = ENEMYTEXTURES[enemyType];
	entities[id].mask = ENEMYTEXTURES[enemyType];
	entities[id].type = 3;
	entities[id].scale = 92 + (ENEMYSIZES[enemyType] * 32);
	entities[id].moving = true;
	entities[id].yOffset = ENEMYOFFSETS[enemyType];
	//192 - entities[id].scale * 2 / 3;
	entities[id].speed = ENEMYSPEEDS[enemyType] / speedModifier;
	entities[id].actionDelay = 20;
	entities[id].actionFrequency = 20;
	entities[id].damage = 10 + enemyLevel + damageModifier;
	entities[id].hp = enemyLevel / 2 + 1 + hpModifier;
	entities[id].hit = 0;
}

/// @brief initialize an item pickup
/// @param id position in entity array
/// @param x tile position (0 to mapsize)
/// @param y
void initPickup(int type, int id, int x, int y)
{

	if (type == 0)
	{ // gun level up
		entities[id].active = true;
		entities[id].x = int2fx(x) + 128;
		entities[id].y = int2fx(y) + 128;
		entities[id].texture = 11;
		entities[id].type = 5;
		entities[id].scale = 128;
		entities[id].moving = false;
		entities[id].yOffset = 32;
		entities[id].hit = 0;
		copyText(entities[id].notification, "SPELL", 5);
		entities[id].notificationLength = 5;
	}
	else if (type == 1)
	{ // healthpack
		entities[id].active = true;
		entities[id].x = int2fx(x) + 128;
		entities[id].y = int2fx(y) + 128;
		entities[id].texture = 12;
		entities[id].type = 4;
		entities[id].scale = 128;
		entities[id].moving = false;
		entities[id].yOffset = 32;
		entities[id].hit = 0;
		entities[id].damage = 50;
		copyText(entities[id].notification, "HEALTH", 6);
		entities[id].notificationLength = 6;
	}
	else if (type == 2)
	{
		entities[id].active = true;
		entities[id].x = int2fx(x) + 128;
		entities[id].y = int2fx(y) + 128;
		entities[id].texture = 18;
		entities[id].type = 6;
		entities[id].scale = 128;
		entities[id].moving = false;
		entities[id].yOffset = 32;
		entities[id].hit = 0;
		copyText(entities[id].notification, "MANA", 4);
		entities[id].notificationLength = 4;
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
	entities[0].yOffset = 32;
	entities[0].hit = 0;
	copyText(entities[0].notification, "KEY", 3);
	entities[0].notificationLength = 3;
}

void initSplatter(int id)
{
	entities[id].active = true;
	entities[id].mask = 19;
	entities[id].type = 7;
	entities[id].hp = 4;
	entities[id].actionDelay = 1;
	entities[id].actionFrequency = 1;
	entities[id].hit = false;
}

/// @brief initialize entity array with unused values
void initEntities()
{
	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		entities[i].active = false;
	}
}

void fire(FIXED dirX, FIXED dirY)
{
	if (player.ammo - 20 >= 0 || player.overdrive)
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
			entities[i].yOffset = 32;
			entities[i].hit = 0;

			playSound(13 + player.gunLevel - 1);
			if (!player.overdrive)
			{
				player.ammo -= 20;
			}

			break;
		}
	}
}

/// @brief disables an entity from being drawn
/// @param i entity array index
void removeEntity(int i)
{
	entities[i].active = false;
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
			if (entities[i].distance < 64)
			{
				setNotification(&entities[i]);
				removeEntity(i);
				player.hasKey = true;
				updateHud = 2;
				VBlankIntrWait();
				playSound(16);
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
						initSplatter(j);
						playSound(12);
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
				if (!entities[i].actionDelay)
				{
					player.hp -= entities[i].damage;
					updateHud = 2;
					entities[i].actionDelay = entities[i].actionFrequency;
					playSound(1);
				}
			}
			if (entities[i].actionDelay)
			{
				entities[i].actionDelay--;
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
					setNotification(&entities[i]);
					removeEntity(i);
					updateHud = 2;
					VBlankIntrWait();
					playSound(2);
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
					setNotification(&entities[i]);
					updateHud = 2;
				}
				removeEntity(i);
				VBlankIntrWait();
				playSound(3);
			}
		}
		else if (type == 6)
		{
			if (entities[i].distance < 64)
			{
				player.overdrive = player.maxOverDrive;
				player.ammo = player.maxAmmo;
				setNotification(&entities[i]);
				// updateHud = 2;
				removeEntity(i);
				VBlankIntrWait();
				playSound(19);
			}
		}
		else if (type == 7)
		{
			entities[i].actionDelay--;
			if (entities[i].actionDelay == 0)
			{
				entities[i].hp--;
				if (entities[i].hp <= 0)
				{
					removeEntity(i);
				}
				else
				{
					entities[i].actionDelay = entities[i].actionFrequency;
					entities[i].mask++;
				}
			}
		}
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

			if (entities[i].distance < 12000 && entities[i].distance > 128)
			{
				//
				const FIXED xDiff = player.x - entities[i].x;
				const FIXED yDiff = player.y - entities[i].y;

				const FIXED len = fxadd(fixedAbs(xDiff), fixedAbs(yDiff));

				const FIXED xU = fxmul(fxdiv(xDiff, len), entities[i].speed);
				const FIXED yU = fxmul(fxdiv(yDiff, len), entities[i].speed);

				const FIXED newX = fxadd(entities[i].x, xU);
				const FIXED newY = fxadd(entities[i].y, yU);

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

int compareDistances(const void *v1, const void *v2)
{
	const struct Entity *u1 = v1;
	const struct Entity *u2 = v2;
	return u1->distance < u2->distance;
}

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

void sortEntities()
{
	quickSort(entityOrder, 0, MAXENTITYCOUNT - 1);
}

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

	for (int i = 0; i < MAXENTITYCOUNT; i++)
	{
		if (!entities[entityOrder[i]].active || entities[entityOrder[i]].distance < 64)
		{
			continue;
		}
		FIXED entityX = fxsub(entities[entityOrder[i]].x, player.x);
		FIXED entityY = fxsub(entities[entityOrder[i]].y, player.y);

		FIXED invDet = RECIPROCALLUT[fxsub(fxmul(planeX, dirY), fxmul(dirX, planeY)) + 350];
		// fxdiv(int2fx(1), fxsub(fxmul(planeX, dirY), fxmul(dirX, planeY)));
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
		if (transformY > 0)
		{

			const int spriteScreenX = fx2int(fxmul(int2fx(SCREENWIDTH >> 1), (fxadd(int2fx(1), fxdiv(transformX, transformY)))));

			const int spriteHeight = fixedAbs(fx2int(fxdiv(fxmul(int2fx(SCREENHEIGHT), entities[entityOrder[i]].scale), (transformY))));

			// crappy method for getting rid of flickers of an unknown bug
			if (spriteHeight > 250)
			{
				continue;
			}

			//fx2int(fxmul(int2fx(spriteHeight), entities[entityOrder[i]].yOffset));
			const int offsetY = fx2int(fxdiv(int2fx(entities[entityOrder[i]].yOffset), transformY));
			// calculate lowest and highest pixel to fill in current stripe
			int drawStartY = -spriteHeight / 2 + SCREENHEIGHT / 2 + offsetY;
			if (drawStartY < 0)
				drawStartY = 0;
			int drawEndY = spriteHeight / 2 + SCREENHEIGHT / 2 + offsetY;
			if (drawEndY >= SCREENHEIGHT)
				drawEndY = SCREENHEIGHT - 1;

			const int spriteWidth = spriteHeight;
			int drawStartX = -spriteWidth / 2 + spriteScreenX;
			int drawEndX = spriteWidth / 2 + spriteScreenX;
			drawStartX /= 2;
			drawEndX /= 2;

			FIXED horizontalTexFrag = fxdiv(int2fx(TEXTURESIZE << 1), int2fx(spriteWidth));
			const int texture = entities[entityOrder[i]].texture;
			const int type = entities[entityOrder[i]].type;

			const int height = spriteHeight;//drawEndY - drawStartY;
			const FIXED yStep = TEXTURESTEPLUT[height];
			// todo: use this
			const int downScaledHeightLimit = 64;

			if (type != 7)
			{
				const int hit = entities[entityOrder[i]].hit;
				if (!hit)
				{
					if (height < downScaledHeightLimit)
					{
						drawSprite(drawStartX, drawEndX, drawStartY, drawEndY, horizontalTexFrag, yStep, transformY, texture);
					}
					else
					{
						drawSpriteDownScaled(drawStartX, drawEndX, drawStartY, drawEndY, horizontalTexFrag, yStep, transformY, texture, 2);
					}
				}
				else
				{
					const int color = 11;
					drawSpriteFlatColor(drawStartX, drawEndX, drawStartY, drawEndY, horizontalTexFrag, yStep, transformY, texture, color);
				}
			}
			else
			{
				const int mask = entities[entityOrder[i]].mask;
				drawSpriteWithMask(drawStartX, drawEndX, drawStartY, drawEndY, horizontalTexFrag, yStep, transformY, texture, mask);
			}
		}

		entities[entityOrder[i]].hit = 0;
	}
}

void refillEnemies()
{

	int count = 0;
	const int adjustedEnemyCount = difficulty > 0 ? goalEnemyCount : easyGoalEnemyCount;
	int firstFreeSlot = -1;
	for (int i = 1; i < MAXENTITYCOUNT; i++)
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

	if (firstFreeSlot >= 1 && count < adjustedEnemyCount)
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
					if (distance > 7 && distance < 13)
					{
						UTILITYMAPDATA[counter] = y * MAPSIZE + x;
						counter++;
					}
				}
			}
		}

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
			if (distance >= PRUNEENEMYDISTANCE)
			{
				removeEntity(i);
			}
		}
	}
}