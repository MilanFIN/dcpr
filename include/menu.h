#include <tonc.h>
#include "render.h"
#include "dungeon.h"

#ifndef MENU_H
#define MENU_H

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

#endif // MENU_H