#include "hud.h"

int updateHud = 2;

char notification[5] = "ABCDE";
size_t notificationLength = 5;
int notificationCounter = 0;
const int notificationDuration = 20;
const int HUDHEIGHT = 160 - SCREENHEIGHT;

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