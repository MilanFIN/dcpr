#ifndef RENDER_H
#define RENDER_H

#include <tonc.h>
#include "textures.h"

#define SCREENHEIGHT 134
#define SCREENWIDTH 240

extern FIXED CAMERAX_LU[SCREENWIDTH / 2];
extern FIXED TEXTURESTEP_LU[SCREENHEIGHT];

/// @brief write a pixel value to video memory (2 pixels due to mode4)
/// @param x x coordinate of the leftmost pixel on screen
/// @param y y coordinate on screen
/// @param clrid color palette index
INLINE void m4_dual_plot(int x, int y, u8 clrid)
{
	vid_page[(y * M4_WIDTH + x) >> 1] = (clrid << 8) | clrid;
}

/// @brief draw a vertical line on screen (2 columns wide)
/// @param x screen column
/// @param y1 start
/// @param y2 end (inclusive)
/// @param clrid color palette index
INLINE void m4_dual_vline(int x, int y1, int y2, u8 clrid)
{
	for (int i = y1; i <= y2; i++)
	{
		m4_dual_plot(x, i, clrid);
	}
}

// draw a textured vertical line to x column, between y1, y2
//  column defines which texture column is used
INLINE void m4_textured_dual_line(const int *textures, int x, int y1, int y2, int type, int vertical, int column, FIXED step, int textureSize)
{
	FIXED textureY = 0;
	for (int y = y1; y < y2; y++)
	{
		const int color = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column] - vertical;
		m4_dual_plot(x, y, color);
		textureY = fxadd(textureY, step);
	}
}

// draw a textured vertical line to x column, between y1, y2
//  column defines which texture column is used
INLINE void m4_reduced_res_textured_dual_line(const int *textures, int x, int y1, int y2, int type, int vertical, int column, FIXED step, int textureSize, int downScale)
{
	FIXED textureY = 0;
	for (int y = y1; y < y2; y += downScale)
	{
		const int color = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column] - vertical;
		for (int i = 0; i < downScale; i++)
		{
			m4_dual_plot(x, y + i, color);
		}
		textureY = fxadd(textureY, downScale * step);
	}
}

// same as above, but supports transparency
INLINE void m4_sprite_textured_dual_line(const int *textures, int x, int y1, int y2, int type, int column, FIXED step, int textureSize)
{
	FIXED textureY = 0;
	for (int y = y1; y < y2; y++)
	{
		const int color = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column];
		if (color != 0)
		{
			m4_dual_plot(x, y, color);
		}
		textureY = fxadd(textureY, step);
	}
}

// same as above, but supports transparency
INLINE void m4_downscaled_sprite_textured_dual_line(const int *textures, int x, int y1, int y2, int type, int column, FIXED step, int textureSize, int downScale)
{
	FIXED textureY = 0;
	for (int y = y1; y < y2; y += downScale)
	{
		const int color = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column];
		if (color != 0)
		{
			for (int i = 0; i < downScale; i++)
			{
				m4_dual_plot(x, y + i, color);
			}
		}
		textureY = fxadd(textureY, downScale * step);
	}
}

// same as above, supports transparency, but all colors are replaced
// with flatColor (palette id)
INLINE void m4_sprite_color_textured_dual_line(const int *textures, int x, int y1, int y2, int type, int column, int flatColor, FIXED step, int textureSize)
{
	FIXED textureY = 0;
	for (int y = y1; y < y2; y++)
	{
		const int realColor = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column];
		if (realColor != 0)
		{
			m4_dual_plot(x, y, flatColor);
		}
		textureY = fxadd(textureY, step);
	}
}

INLINE void m4_downscaled_sprite_color_textured_dual_line(const int *textures, int x, int y1, int y2, int type, int column, int flatColor, FIXED step, int textureSize, int downScale)
{
	FIXED textureY = 0;
	for (int y = y1; y < y2; y += downScale)
	{
		const int realColor = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column];
		if (realColor != 0)
		{
			for (int i = 0; i < downScale; i++)
			{
				m4_dual_plot(x, y+i, flatColor);
			}
		}
		textureY = fxadd(textureY, step * downScale);
	}
}

// draws a rectangular texture to arbitary point on screen
// texture as in id from TEXTURES
// scale: 0 for 16px, 1 for 8, 2 for 4 etc
void drawFlat(const int *textures, int texture, int x, int y, int w, int h, int scale, int textureSize);

// draws a rectangular texture to arbitary point on screen
// flipped on the x axis
// texture as in id from TEXTURES
// scale: 0 for 16px, 1 for 8, 2 for 4 etc
void drawFlatMirrored(const int *textures, int texture, int x, int y, int w, int h, int scale, int textureSize);

// same as above, but use a single color for visible parts of the texture
void drawFlatColorTexture(const int *textures, int texture, int x, int y, int w, int h, int color, int scale, int textureSize);

// writes an individual letter (number or capital) to a specific position on screen
void writeLetter(char *letter, int x, int y, int color);

// writes a line of text (number or capital) to a position on screen with a color based on palette index
void writeLine(char *content, char length, int x, int y, int color);

void fillArea(int x0, int y0, int x1, int y1, char color);

#endif // RENDER_H
