#include "raycaster.h"

const int HALFSCREENPOINT = SCREENHEIGHT / 2;
FIXED zBuffer[SCREENWIDTH / 2] = {0};
const int CASTEDRAYS = SCREENWIDTH / 2;

FIXED dirX, dirY;
FIXED planeX, planeY;

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
	if (distance > 1024)
	{
		m4_textured_dual_line(TEXTURES, i, HALFSCREENPOINT - halfHeight, HALFSCREENPOINT + halfHeight, type, vertical, textureColumn, yStep, TEXTURESIZE);
	}
	else
	{
		m4_reduced_res_textured_dual_line(TEXTURES, i, HALFSCREENPOINT - halfHeight, HALFSCREENPOINT + halfHeight, type, vertical, textureColumn, yStep, TEXTURESIZE, 2);
	}
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
			else if (side == 1 && rayDirY > 0)
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
