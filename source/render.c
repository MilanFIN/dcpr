#include "render.h"

FIXED CAMERAX_LU[SCREENWIDTH / 2] = {0};
FIXED TEXTURESTEP_LU[SCREENHEIGHT] = {0};

void drawFlat(const int *textures, int texture, int x, int y, int w, int h, int scale, int textureSize)
{
	// scale h to account for width being at half resolution
	h = h << 1;
	FIXED textureX = 0;
	const FIXED xStep = TEXTURESTEP_LU[w] >> scale;
	const FIXED yStep = TEXTURESTEP_LU[h] >> scale;
	int maxW = CLAMP(x + w, 0, 121);

	for (int x1 = x; x1 < maxW; x1++)
	{
		m4_sprite_textured_dual_line(textures, 2 * x1, y, y + h, texture, fx2int(textureX), yStep, textureSize);
		textureX = fxadd(textureX, xStep);
	}
}

void drawFlatMirrored(const int *textures, int texture, int x, int y, int w, int h, int scale, int textureSize)
{
	// scale h to account for width being at half resolution
	h = h << 1;
	FIXED textureX = int2fx(15);
	const FIXED xStep = TEXTURESTEP_LU[w] >> scale;
	const FIXED yStep = TEXTURESTEP_LU[h] >> scale;
	int maxW = CLAMP(x + w, 0, 121);

	for (int x1 = x; x1 < maxW; x1++)
	{
		m4_sprite_textured_dual_line(textures, 2 * x1, y, y + h, texture, fx2int(textureX), yStep, textureSize);
		textureX = fxsub(textureX, xStep);
	}
}

void drawFlatColorTexture(const int *textures, int texture, int x, int y, int w, int h, int color, int scale, int textureSize)
{
	// h = h << 1;
	FIXED textureX = 0;
	const FIXED xStep = TEXTURESTEP_LU[w] >> scale;
	const FIXED yStep = TEXTURESTEP_LU[h] >> scale; // >> (2*scale);

	for (int x1 = x; x1 < x + w; x1++)
	{
		m4_sprite_color_textured_dual_line(textures, 2 * x1, y, y + h, texture, fx2int(textureX), color, yStep, textureSize);
		textureX = fxadd(textureX, xStep);
	}
}

void writeLetter(char *letter, int x, int y, int color)
{
	// shifting to match the existing letter array
	char characterIndex = letter[0] - 47;
	drawFlatColorTexture(LETTERS, characterIndex, x, y, 8, 8, color, 1, LETTERSIZE);
}

void writeLine(char *content, char length, int x, int y, int color)
{
	for (int i = 0; i < length; i++)
	{
		writeLetter(content + i, x + 9 * i, y, color);
	}
}

void fillArea(int x0, int y0, int x1, int y1, char color)
{
	for (int x = x0; x <= x1; x++)
	{
		m4_dual_vline(x, y0, y1, color);
	}
}

void shiftPixelDown(int x, int y, char amount)
{
	vid_page[((y + amount) * M4_WIDTH + x) >> 1] = vid_page[(y * M4_WIDTH + x) >> 1];
}

void endAnimation(char backgroundColor)
{
	char speeds[120] = {0};
	char top[240] = {0};
	for (int i = 0; i < 120; i++)
	{
		speeds[i] = qran_range(0, 5);
	}

	for (int i = 0; i < 40; i++)
	{
		for (int x = 0; x < 240; x += 2)
		{
			char amount = speeds[x >> 1];
			for (int y = 159; y > 0; y--)
			{
				if (y + amount < 159)
				{
					shiftPixelDown(x, y, amount);
				}
			}
			top[(x >> 1)] += amount;
			m4_dual_vline(x, 0, top[(x >> 1)], backgroundColor);
		}
		vid_flip();
	}
}