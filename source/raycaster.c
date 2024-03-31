#include "raycaster.h"

const int HALFSCREENPOINT = SCREENHEIGHT / 2;

FIXED dirX, dirY;
FIXED planeX, planeY;

void drawWall(int i, FIXED distance, int type, int vertical, int textureColumn)
{

	int wallHeight = fx2int(fxdiv(int2fx(SCREENHEIGHT), distance));
	wallHeight = CLAMP(wallHeight, 1, SCREENHEIGHT + 4);
	const int halfHeight = (wallHeight >> 1);
	const int startY = HALFSCREENPOINT - halfHeight;
	const int endY = HALFSCREENPOINT + halfHeight;

	// roof
	m4_dual_vline(i, 0, startY, ROOFCOLOR);

	const FIXED yStep = TEXTURESTEPLUT[wallHeight];

	// the actual wall
	if (distance > 1024)
	{
		m4_textured_dual_line(TEXTURES, i, startY, endY, type, vertical, textureColumn, yStep, TEXTURESIZE);
	}
	else
	{
		m4_reduced_res_textured_dual_line(TEXTURES, i, startY, endY, type, vertical, textureColumn, yStep, TEXTURESIZE, 2);
	}
	m4_dual_vline(i, endY, SCREENHEIGHT, FLOORCOLOR);
}

void drawWallCroppedTop(int i, FIXED distance, int type, int vertical, int textureColumn)
{

	const int cropPoint = 32;

	int wallHeight = fx2int(fxdiv(int2fx(SCREENHEIGHT), distance));
	wallHeight = CLAMP(wallHeight, 1, SCREENHEIGHT + 4);
	const int halfHeight = (wallHeight >> 1);
	FIXED yStep = TEXTURESTEPLUT[wallHeight];
	int skipNFirst = 0;

	int startY = HALFSCREENPOINT - halfHeight;
	const int endY = HALFSCREENPOINT + halfHeight;

	// roof only draw the part visible after crop point
	if (startY > cropPoint)
	{
		m4_dual_vline(i, cropPoint, HALFSCREENPOINT - halfHeight, ROOFCOLOR);
	}
	if (startY < cropPoint)
	{
		skipNFirst = cropPoint - startY;
		startY = cropPoint;
	}

	m4_textured_dual_line_skip_first_y(TEXTURES, i, startY, endY, type, vertical, textureColumn, yStep, TEXTURESIZE, skipNFirst);

	m4_dual_vline(i, endY, SCREENHEIGHT, FLOORCOLOR);
}

void drawWallCroppedBottom(int i, FIXED distance, int type, int vertical, int textureColumn)
{

	const int cropPoint = SCREENHEIGHT - 18;

	int wallHeight = fx2int(fxdiv(int2fx(SCREENHEIGHT), distance));
	wallHeight = CLAMP(wallHeight, 1, SCREENHEIGHT + 4);
	const int halfHeight = (wallHeight >> 1);
	const FIXED yStep = TEXTURESTEPLUT[wallHeight];

	const int startY = HALFSCREENPOINT - halfHeight;
	const int endY = HALFSCREENPOINT + halfHeight;
	int adjustedEndY = endY;

	m4_dual_vline(i, 0, startY, ROOFCOLOR);
	if (endY > cropPoint)
	{
		adjustedEndY = cropPoint;
	}
	m4_textured_dual_line(TEXTURES, i, startY, adjustedEndY, type, vertical, textureColumn, yStep, TEXTURESIZE);

	if (endY < cropPoint)
	{
		m4_dual_vline(i, endY, cropPoint, FLOORCOLOR);
	}
}

void drawWithoutWall(int i)
{
	// roof
	m4_dual_vline(i, 0, HALFSCREENPOINT, ROOFCOLOR);
	// floor
	m4_dual_vline(i, HALFSCREENPOINT, SCREENHEIGHT, FLOORCOLOR);
}

void castRays()
{
	for (int i = 0; i < CASTEDRAYS; i++)
	{

		const FIXED cameraX = CAMERAXLUT[i]; // x-coordinate in camera space
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
			// deltaDistX = fixedAbs(fxdiv(int2fx(1), rayDirX));
			deltaDistX = fixedAbs(RECIPROCALLUT[rayDirX + 350]);
		}

		if (rayDirY == 0)
		{
			deltaDistY = int2fx(1024);
		}
		else
		{
			// deltaDistY = fixedAbs(fxdiv(int2fx(1), rayDirY));
			deltaDistY = fixedAbs(RECIPROCALLUT[rayDirY + 350]);
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
			sideDistX = fxmul(fxsub(fxadd(mapX, int2fx(1)), player.x), deltaDistX);
		}
		if (rayDirY < 0)
		{
			stepY = int2fx(-1);
			sideDistY = fxmul(fxsub(player.y, mapY), deltaDistY);
		}
		else
		{
			stepY = int2fx(1);
			sideDistY = fxmul(fxsub(fxadd(mapY, int2fx(1)), player.y), deltaDistY);
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
			else if (side == 1 && rayDirY > 0)
				textureColumn = TEXTURESIZE - textureColumn - 1;

			zBuffer[i] = perpWallDistance;

			if (i > 54 || (/*i > 15 &&*/ i < 25))
			{
				drawWall(i << 1, perpWallDistance, MAP[yCell * MAPSIZE + xCell], side, textureColumn);
			}
			/*
			else if (i < 16)
			{
				drawWallCroppedTop(i << 1, perpWallDistance, MAP[yCell * MAPSIZE + xCell], side, textureColumn);
			}*/
			else
			{
				drawWallCroppedBottom(i << 1, perpWallDistance, MAP[yCell * MAPSIZE + xCell], side, textureColumn);
			}
		}
		/*
		else
		{
			drawWithoutWall(i << 1);
		}*/
	}
}

/// @brief cast a ray forward in 2d plane, and check if specified wall is hit
/// @param targetType texture of target wall
/// @return true, if specified target was hit
int castRay(int targetType, FIXED dirX, FIXED dirY)
{
	const FIXED cameraX = CAMERAXLUT[CASTEDRAYS >> 1]; // x-coordinate in camera space
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

/// @brief check if player is looking at anything special (walls)
bool castForward(FIXED dirX, FIXED dirY)
{
	// check if player has key and is looking at a door
	int distance = castRay(5, dirX, dirY);
	if (distance >= 0 && distance < 2)
	{

		if (player.hasKey)
		{
			VBlankIntrWait();
			playSound(18);
			return true;
		}
		else
		{
			VBlankIntrWait();
			playSound(17);
			return false;
		}
	}
	return false;
}