#ifndef RENDER_H
#define RENDER_H

#include <tonc.h>
#include "textures.h"
#include "lut.h"

#define SCREENHEIGHT 134
#define SCREENWIDTH 240

extern const int CASTEDRAYS;
extern FIXED zBuffer[SCREENWIDTH / 2];

/// @brief write value to video memory (2 horizontal pixels due to mode4)
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

/// @brief draw a textured vertical line on the screen
/// @param textures texture source array
/// @param x screen column
/// @param y1 start of the line
/// @param y2 end of the line
/// @param type index of texture in texture array (1 indexed)
/// @param vertical 0-1, only used on map walls to create a shade difference between normal directions, otherwise 0
/// @param column column of the texture
/// @param step how many pixels in texture space is a single step in the screen space
/// @param textureSize texture resolution, 8 or 16 depending on textures pointer value
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

INLINE void m4_textured_dual_line_skip_first_y(const int *textures, int x, int y1, int y2, int type, int vertical, int column, FIXED step, int textureSize, int skipFirstY)
{
	FIXED textureY = fxmul(int2fx(skipFirstY), step);
	for (int y = y1; y < y2; y++)
	{
		const int color = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column] - vertical;
		m4_dual_plot(x, y, color);
		textureY = fxadd(textureY, step);
	}
}

/// @brief similar to m4_textured_dual_line, but with half vertical resolution
/// @param textures texture source array
/// @param x screen column
/// @param y1 start of the line
/// @param y2 end of the line
/// @param type index of texture in texture array (1 indexed)
/// @param vertical 0-1, only used on map walls to create a shade difference between normal directions, otherwise 0
/// @param column column of the texture
/// @param step how many pixels in texture space is a single step in the screen space
/// @param textureSize texture resolution, 8 or 16 depending on textures pointer value
/// @param downScale how many times the resolution is reduced, eg. 2 for half
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

/// @brief A version of m4_textured_dual_line, that supports transparency (0 == transparent)
/// @param textures texture source array
/// @param x screen column
/// @param y1 start of the line
/// @param y2 end of the line
/// @param type index of texture in texture array (1 indexed)
/// @param column column of the texture
/// @param step how many pixels in texture space is a single step in the screen space
/// @param textureSize texture resolution, 8 or 16 depending on textures pointer value
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

INLINE void m4_sprite_textured_dual_line_skip_first_y(const int *textures, int x, int y1, int y2, int type, int column, FIXED step, int textureSize, int skipFirstY)
{
	FIXED textureY = fxmul(int2fx(skipFirstY), step);
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

/// @brief A version of m4_sprite_textured_dual_line, that suppots reduced resolution
/// @param textures texture source array
/// @param x screen column
/// @param y1 start of the line
/// @param y2 end of the line
/// @param type index of texture in texture array (1 indexed)
/// @param column column of the texture
/// @param step how many pixels in texture space is a single step in the screen space
/// @param textureSize texture resolution, 8 or 16 depending on textures pointer value
/// @param downScale how many times the resolution is reduced, eg. 2 for half
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

/// @brief renders a vertical line by using the texture as a mask for color and transparency
/// @param textures texture source array
/// @param x screen column
/// @param y1 start of the line
/// @param y2 end of the line
/// @param type index of texture in texture array (1 indexed)
/// @param column column of the texture
/// @param flatColor palette index of the color
/// @param step how many pixels in texture space is a single step in the screen space
/// @param textureSize texture resolution, 8 or 16 depending on textures pointer value
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

INLINE void m4_sprite_color_textured_single_line(const int *textures, int x, int y1, int y2, int type, int column, int flatColor, FIXED step, int textureSize)
{
	FIXED textureY = 0;
	for (int y = y1; y <= y2; y++)
	{
		const int realColor = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column];
		if (realColor != 0)
		{
			m4_plot(x, y, flatColor);
		}
		textureY = fxadd(textureY, step);
	}
}

INLINE void m4_sprite_masked_textured_dual_line(const int *textures, int x, int y1, int y2, int type, int mask, int column, FIXED step, int textureSize)
{
	FIXED textureY = 0;
	for (int y = y1; y < y2; y++)
	{
		const int color = textures[(type - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column];
		const int maskColor = textures[(mask - 1) * textureSize * textureSize + fx2int(textureY) * textureSize + column];
		if (maskColor != 0 && color != 0)
		{
			m4_dual_plot(x, y, color);
		}
		textureY = fxadd(textureY, step);
	}
}

INLINE void drawSprite(int startX, int endX, int startY, int endY, FIXED xStep, FIXED yStep, FIXED transformY, int texture)
{
	FIXED hTexPos = 1;
	for (int stripe = startX; stripe < endX; stripe++)
	{
		if (stripe >= 0 && stripe < SCREENWIDTH / 2)
		{
			if (transformY < zBuffer[stripe])
			{
				const int texX = fx2int(hTexPos);
				m4_sprite_textured_dual_line(TEXTURES, 2 * stripe, startY, endY, texture, texX, yStep, TEXTURESIZE);
			}
		}
		else if (stripe > SCREENWIDTH / 2)
		{
			break;
		}
		hTexPos = fxadd(hTexPos, xStep);
	}
}

INLINE void drawSpriteDownScaled(int startX, int endX, int startY, int endY, FIXED xStep, FIXED yStep, FIXED transformY, int texture, int downscale)
{
	FIXED hTexPos = 1;
	for (int stripe = startX; stripe < endX; stripe++)
	{
		if (stripe >= 0 && stripe < SCREENWIDTH / 2)
		{
			if (transformY < zBuffer[stripe])
			{
				const int texX = fx2int(hTexPos);
				m4_downscaled_sprite_textured_dual_line(TEXTURES, 2 * stripe, startY, endY, texture, texX, yStep, TEXTURESIZE, downscale);
			}
		}
		else if (stripe > SCREENWIDTH / 2)
		{
			break;
		}
		hTexPos = fxadd(hTexPos, xStep);
	}
}

INLINE void drawSpriteWithMask(int startX, int endX, int startY, int endY, FIXED xStep, FIXED yStep, FIXED transformY, int texture, int mask)
{
	FIXED hTexPos = 1;
	for (int stripe = startX; stripe < endX; stripe++)
	{
		if (stripe >= 0 && stripe < SCREENWIDTH / 2)
		{
			if (transformY < zBuffer[stripe])
			{
				const int texX = fx2int(hTexPos);
				m4_sprite_masked_textured_dual_line(TEXTURES, 2 * stripe, startY, endY, texture, mask, texX, yStep, TEXTURESIZE);
			}
		}
		else if (stripe > SCREENWIDTH / 2)
		{
			break;
		}
		hTexPos = fxadd(hTexPos, xStep);
	}
}

INLINE void drawSpriteFlatColor(int startX, int endX, int startY, int endY, FIXED xStep, FIXED yStep, FIXED transformY, int texture, int color)
{
	FIXED hTexPos = 1;
	for (int stripe = startX; stripe < endX; stripe++)
	{
		if (stripe >= 0 && stripe < SCREENWIDTH / 2)
		{
			if (transformY < zBuffer[stripe])
			{
				const int texX = fx2int(hTexPos);
				m4_sprite_color_textured_dual_line(TEXTURES, 2 * stripe, startY, endY, texture, texX, color, yStep, TEXTURESIZE);
			}
		}
		else if (stripe > SCREENWIDTH / 2)
		{
			break;
		}
		hTexPos = fxadd(hTexPos, xStep);
	}
}

/// @brief draw a flat texture on screen
/// @param textures source array for textures (1d buffer)
/// @param texture texture index in array
/// @param x top left coordinate on screen
/// @param y -||-
/// @param w width of texture (texture will be 2x as big due to half res rendering)
/// @param h height
/// @param scale 1 for 8px wide source texture, 0 for 16
/// @param textureSize 8 or 16
void drawFlat(const int *textures, int texture, int x, int y, int w, int h, int scale, int textureSize);

/// @brief Same as drawFlat, but flipped horizontally
void drawFlatMirrored(const int *textures, int texture, int x, int y, int w, int h, int scale, int textureSize);

/// @brief Same as drawFlat, but using texture as a mask for single color
/// @param color color palette index
void drawFlatColorTexture(const int *textures, int texture, int x, int y, int w, int h, int color, int scale, int textureSize);

void drawFlatMasked(const int *textures, int texture, int mask, int x, int y, int w, int h, int scale, int textureSize);

/// @brief Same as drawFlat, but using full res instead of 1/2
/// @param color color palette index
void drawFlatColorFullResolution(const int *textures, int texture, int x, int y, int w, int h, int color, int scale, int textureSize);

/// @brief write a letter on screen
/// @param letter supported: A-Z, 0-9
/// @param x top left coordinate
/// @param y -||-
/// @param color color palette index
void writeLetter(char *letter, int x, int y, int color);

// writes a line of text (number or capital) to a position on screen with a color based on palette index

/// @brief Write a line of characters
/// @param content char array (A-Z, 0-9)
/// @param length character count
/// @param x top left coordinate
/// @param y -||-
/// @param color color palette index
void writeLine(char *content, char length, int x, int y, int color);

/// @brief fill an area on screen with flat color
void fillArea(int x0, int y0, int x1, int y1, char color);

void shiftPixelDown(int x, int y, char amount);

void endAnimation(char backgroundColor);

#endif // RENDER_H
