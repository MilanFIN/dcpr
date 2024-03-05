#include "hud.h"

const int notificationDuration = 20;
const int HUDHEIGHT = 160 - SCREENHEIGHT;

int updateHud = 2;
int updateCompass = 2;
char notification[5] = "ABCDE";
size_t notificationLength = 5;
int notificationCounter = 0;
int compassDirection = 1;

void setNotification(struct Entity *entity)
{
	notificationCounter = notificationDuration;
	notificationLength = entity->notificationLength;
	copyText(notification, entity->notification, entity->notificationLength);
}

/// @brief draw player hud and contents
void drawHud()
{
	// background
	fillArea(1, 160 - HUDHEIGHT + 2, 2 * CASTEDRAYS - 4, 158, 29);
	// borders
	fillArea(0, 160 - HUDHEIGHT + 1, 2 * CASTEDRAYS, 160 - HUDHEIGHT + 2, 26);
	fillArea(0, 158, 2 * CASTEDRAYS, 160, 26);
	fillArea(0, 160 - HUDHEIGHT, 1, 160, 26);
	fillArea(238, 160 - HUDHEIGHT, 240, 160, 26);

	// key icon and border
	if (player.hasKey)
	{
		drawFlat(TEXTURES, 3, 19, 160 - HUDHEIGHT - 10, 16, 16, 0, TEXTURESIZE);
	}
	fillArea(74, 160 - HUDHEIGHT + 1, 76, 159, 26);

	// health bar
	fillArea(92, 160 - HUDHEIGHT + 5, 92 + player.hp / 2, 160 - 5, 15);
	fillArea(92 + player.hp / 2, 160 - HUDHEIGHT + 5, 141, 160 - 5, 27);

	fillArea(158, 160 - HUDHEIGHT + 1, 160, 159, 26);

	// gun icon and border
	fillArea(30, 160 - HUDHEIGHT + 1, 32, 159, 26);
	drawFlat(TEXTURES, PROJECTILETEXTURES[player.gunLevel - 1], 2, 160 - HUDHEIGHT + 3, 12, 10, 0, TEXTURESIZE);
}

void drawCompass()
{
	drawFlat(COMPASSDIRECTIONS, compassDirection, 0, 0, 16, 16, 0, TEXTURESIZE);
}

void updateAmmo()
{
	if (!player.overdrive)
	{
		fillArea(174, 160 - HUDHEIGHT + 5, 174 + player.ammo / 2 - 1, 160 - 5, 31);
		fillArea(174 + player.ammo / 2, 160 - HUDHEIGHT + 5, 223, 160 - 5, 27);
		if (player.ammo < player.maxAmmo)
		{
			player.ammo += 1;
		}
	}
	else
	{
		fillArea(174, 160 - HUDHEIGHT + 5, 174 + player.overdrive / 4 - 1, 160 - 5, 47);
		fillArea(174 + player.overdrive / 4, 160 - HUDHEIGHT + 5, 223, 160 - 5, 27);
	}
}

void checkCompass()
{
	int newDirection;
	const int directionDeg = fx2int(player.direction);

	if (directionDeg > 22 && directionDeg < 67)
	{
		newDirection = 2;
	}
	else if (directionDeg < 22 || directionDeg > 338)
	{
		newDirection = 3;
	}
	else if (directionDeg > 292)
	{
		newDirection = 4;
	}
	else if (directionDeg > 247)
	{
		newDirection = 5;
	}
	else if (directionDeg > 202)
	{
		newDirection = 6;
	}
	else if (directionDeg > 157)
	{
		newDirection = 7;
	}
	else if (directionDeg > 112)
	{
		newDirection = 8;
	}
	else
	{
		newDirection = 1;
	}

	if (newDirection != compassDirection)
	{
		compassDirection = newDirection;
		updateCompass = 2;
	}
}