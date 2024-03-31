#include "toplayer.h"

int offsetFrame = 0;
const int HANDXOFFSETS[OFFSETFRAMES] = {0, 1, 2, 3, 3, 2, 1, 0, -1, -2, -3, -3, -2, -1};
const int HANDYOFFSETS[OFFSETFRAMES] = {0, 1, 2, 3, 3, 2, 1, 0, 1, 2, 3, 3, 2, 1};

int movedThisFrame = 0;

void moveHands()
{
	if (movedThisFrame == 0)
	{
		movedThisFrame = 1;
		offsetFrame++;
		if (offsetFrame >= OFFSETFRAMES)
		{
			offsetFrame = 0;
		}
	}
}

void drawHands()
{
	movedThisFrame = 0;
	const int xOffset = HANDXOFFSETS[offsetFrame];
	const int yOffset = HANDYOFFSETS[offsetFrame];

	const int top = 160 - HUDHEIGHT - 25;
	const int bot = top + 26;
	const int hLeft = 140;
	const int bLeft = 46;

	for (int x = 0; x < 25; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			int val = FPSHAND[25 * y + x];
			if (val != 0)
			{
				const int x2 = 2 * x + hLeft + xOffset;
				const int y2 = 2 * y + top + yOffset;
				if (y2 < bot)
					m4_dual_plot(x2, y2, val);
				if (y2 + 1 < bot)
					m4_dual_plot(x2, y2 + 1, val);
			}
		}
	}

	for (int x = 0; x < 35; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			int val = FPSBOOK[35 * y + x];
			if (val != 0)
			{
				const int x2 = 2 * x + bLeft + xOffset;
				const int y2 = 2 * y + top + yOffset;
				if (y2 < bot)
					m4_dual_plot(x2, y2, val);
				if (y2 + 1 < bot)
					m4_dual_plot(x2, y2 + 1, val);
			}
		}
	}
}