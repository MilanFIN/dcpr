
#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <tonc.h>
#include "render.h"
#include "structs.h"
#include "utils.h"
#include "dungeon.h"


extern const int HALFSCREENPOINT;
extern FIXED zBuffer[SCREENWIDTH / 2];
extern const int CASTEDRAYS;

extern FIXED dirX, dirY;
extern FIXED planeX, planeY;

/// @brief draw a single pixel column on screen, inc. roof, wall and floor
/// @param i column (0-120)
/// @param distance distance to the wall (determines wall height)
/// @param type wall texture (indexed from 1)
/// @param vertical true if wall is vertical in 2d map space
/// @param textureColumn which column of the texture is to be read
void drawWall(int i, FIXED distance, int type, int vertical, int textureColumn);


void drawWithoutWall(int i);

/// @brief main raycast method. Loops through the columns of screen (120, when at half resolution)
void castRays();

#endif // RAYCASTER_H