
#include <tonc.h>
#include "textures.h"
#include "structs.h"

#define TILESIZE 1
#define SCREENHEIGHT 135
#define SCREENWIDTH 240
#define FOV 60
#define MAXENTITYCOUNT 10

//todo: fix black screen when direction == int2fx(45);

const float PI = 3.1415;
const int MAPSIZE = 8;

int MAP[8*8] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 4, 1, 1, 1
    };

FIXED CAMERAX_LU[SCREENWIDTH / 2] = {0};
FIXED TEXTURESTEP_LU[SCREENHEIGHT] = {0};


const int CASTEDRAYS = SCREENWIDTH / 2;
const int HALFFOV = FOV / 2;
const int TILESHIFTER = log2(TILESIZE); 
const FIXED FIXEDTILESIZE = TILESIZE * 256; // equal to int2fx(TILESIZE);
const int HALFSCREENPOINT = SCREENHEIGHT / 2;
const int HUDHEIGHT = 160-SCREENHEIGHT;


FIXED x, y;
FIXED dirX, dirY;
FIXED planeX, planeY;
FIXED direction;

struct Entity entities[MAXENTITYCOUNT];
struct Player player;

int entityOrder[MAXENTITYCOUNT] = {0, 1, 2, 3, 4, 5};
FIXED zBuffer[SCREENWIDTH/2] = {0};


volatile unsigned short* palette = (volatile unsigned short*) 0x5000000;
int nextPaletteIndex = 0;


INLINE void m4_dual_plot(int x, int y, u8 clrid)
{
	vid_page[(y*M4_WIDTH+x)>>1] = (clrid << 8) | clrid;
}

INLINE void m4_dual_vline(int x, int y1, int y2, u8 clrid) {
	for (int i = y1; i <= y2; i++) {
		m4_dual_plot(x, i, clrid);
	}
}

INLINE void m4_textured_dual_line(int x, int y1, int y2, int height, int type, int vertical, int column) {
	const FIXED step = TEXTURESTEP_LU[height];
	FIXED textureY = 0;
	for (y1; y1 < y2; y1++) {
		const color = TEXTURES[(type-1) * 256 + fx2int(textureY) * TEXTURESIZE  + column] - vertical;
		m4_dual_plot(x, y1, color);
		textureY = fxadd(textureY, step);
	}
}

INLINE void m4_sprite_textured_dual_line(int x, int y1, int y2, int height, int type, int column) {

	const FIXED step = TEXTURESTEP_LU[height];
	FIXED textureY = 0;
	for (y1; y1 < y2; y1++) {
		const color = TEXTURES[(type-1) * 256 + fx2int(textureY) * TEXTURESIZE  + column];
		if (color != 0) {
			m4_dual_plot(x, y1, color);
		}
		textureY = fxadd(textureY, step);

	}
}


void loadColorToPalette(COLOR c) {
	palette[nextPaletteIndex] = c;
	nextPaletteIndex++;
}

void initShadeOfColor(float r, float g, float b) {
	for (int i = 0; i < 8; i++) {
	loadColorToPalette(RGB15(4*i*r,4*i*g,4*i*b));
	}
}

void initPalette() {
	initShadeOfColor(1,1,1);
	initShadeOfColor(1,0,0);
	initShadeOfColor(0,1,0);
	initShadeOfColor(0,0,1);
	initShadeOfColor(1,1,0);
	initShadeOfColor(1,0,1);
	initShadeOfColor(0,1,1);
	initShadeOfColor(1,0.5,0);
	initShadeOfColor(1,0,0.5);
	initShadeOfColor(0,1,0.5);
	initShadeOfColor(0,0.5,1);
	initShadeOfColor(0.5,0,1);
	initShadeOfColor(0.5,1,0);
	initShadeOfColor(1,0.5,0.5);
	initShadeOfColor(0.5,1,0.5);
	initShadeOfColor(0.5,0.5,1);
	initShadeOfColor(1,0.6,0.3);

}

void initCameraXLu() {
	for (int i = 0; i < CASTEDRAYS; i++) {
		CAMERAX_LU[i] = fxsub(fxdiv(int2fx(2 * i) ,int2fx(CASTEDRAYS )) , int2fx(1));

	}
}

void initTextureStepLu() {
	for (int i = 0; i < SCREENHEIGHT; i++) {
		TEXTURESTEP_LU[i] = fxdiv(int2fx(TEXTURESIZE), int2fx(i));
	}
}

void initEntities() {

	for(int i = 0; i < MAXENTITYCOUNT; i++) {
		entities[i].active = false;
		
	}

	entities[0].active = true;
	entities[0].x = int2fx(4);
	entities[0].y = int2fx(4);
	entities[0].texture = 2;
	entities[0].type = 1;
	entities[0].scale = 128;
	entities[0].moving = false;
	entities[0].yOffset = 128;


	entities[1].active = true;
	entities[1].x = int2fx(5);
	entities[1].y = int2fx(5);
	entities[1].texture = 5;
	entities[0].type = 2;
	entities[1].scale = 128;
	entities[1].moving = false;
	entities[1].yOffset = -256;

}

void initHud() {
	for (int i = 0; i < CASTEDRAYS; i++) {
		m4_dual_vline(2*i, 160-HUDHEIGHT, 160, 29);

	}

}

void initLevel() {

	player.hp = 100;
	player.hasKey = false;

	initEntities();

	x = int2fx(2);//96;//2*64;//
	y = int2fx(2);//224;//2*64;//

	direction = int2fx(0);//int2fx(45);

}


inline float floatAbs(float a) {
	if (a < 0.0) {
		return -a;
	}
	else {
		return a;
	}
}

inline FIXED fixedAbs(FIXED a) {
	if (a < 0) {
		return fxsub(int2fx(0), a);
	}
	else {
		return a;
	}
}


void drawWall(int i, FIXED distance, int type, int vertical, int textureColumn) {

	int wallHeight = fx2int(fxdiv(int2fx(SCREENHEIGHT) , distance));
	wallHeight = CLAMP(wallHeight, 1, SCREENHEIGHT);
	int halfHeight = (wallHeight >> 1);

	//roof
	m4_dual_vline(i, 0, HALFSCREENPOINT-halfHeight, 1);

	int color = 0;
	
	//the actual wall
	m4_textured_dual_line(i, HALFSCREENPOINT-halfHeight, HALFSCREENPOINT + halfHeight, wallHeight, type, vertical, textureColumn);
	//floor
	m4_dual_vline(i, HALFSCREENPOINT+ halfHeight, SCREENHEIGHT, 4);


}


int castRay(int targetType) {
	const FIXED cameraX = CAMERAX_LU[CASTEDRAYS/2]; //x-coordinate in camera space
	const FIXED rayDirX = fxadd(dirX, fxmul(planeX, cameraX));
	const FIXED rayDirY = fxadd(dirY, fxmul(planeY, cameraX));

	FIXED mapX = int2fx(fx2int(x));
	FIXED mapY = int2fx(fx2int(y));

	FIXED sideDistX;
	FIXED sideDistY;

	FIXED deltaDistX; 
	FIXED deltaDistY;
	if (rayDirX == 0) {
		deltaDistX = int2fx(1024);
	}
	else {
		deltaDistX = fixedAbs(fxdiv(int2fx(1), rayDirX));
	}

	if (rayDirY == 0) {
		deltaDistY = int2fx(1024);
	}
	else {
		deltaDistY = fixedAbs(fxdiv(int2fx(1), rayDirY));
	}


	FIXED stepX;
	FIXED stepY;

	int side;

	if(rayDirX < 0) {
		stepX = int2fx(-1);
		sideDistX = fxmul(fxsub(x, mapX) , deltaDistX);
	}
	else {
		stepX = int2fx(1);
		sideDistX = fxmul(fxsub(fxadd(mapX, int2fx(1.0)), x), deltaDistX);
	}
	if(rayDirY < 0) {
		stepY = int2fx(-1);
		sideDistY = fxmul(fxsub(y, mapY), deltaDistY);
	}
	else {
		stepY = int2fx(1);
		sideDistY = fxmul(fxsub(fxadd(mapY, int2fx(1.0)), y), deltaDistY);
	}


	int hit = 0;
	for (int j = 0; j < 100; j++) {
		if(sideDistX < sideDistY) {
			sideDistX = fxadd(sideDistX, deltaDistX);
			mapX = fxadd(mapX, stepX);
			side = 0;
		}
		else {
			sideDistY = fxadd(sideDistY, deltaDistY);
			mapY = fxadd(mapY, stepY);
			side = 1;
		}

		int currentXCell = fx2int(mapX);
		int currentYCell = fx2int(mapY); 
		if(MAP[currentXCell*MAPSIZE + currentYCell] > 0) {
			if (MAP[currentXCell*MAPSIZE + currentYCell] == targetType) {
				hit = 1;
			}
			break;
		}

	}
	
	FIXED perpWallDistance = -256;

	if (hit) {
		if(side == 0) {
			perpWallDistance = fxsub(sideDistX, deltaDistX);
		}
		//vertical
		else {
			perpWallDistance = fxsub(sideDistY, deltaDistY);
		}
	}

	return fx2int(perpWallDistance);

}

int castRays() {
	//pi2 equivalent for calling lu_sin & cos
	//must be scaled down to avoid overflow later


	for (int i= 0; i < CASTEDRAYS; i++) {


		const FIXED cameraX = CAMERAX_LU[i]; //x-coordinate in camera space
		const FIXED rayDirX = fxadd(dirX, fxmul(planeX, cameraX));
		const FIXED rayDirY = fxadd(dirY, fxmul(planeY, cameraX));
		
		
		FIXED mapX = int2fx(fx2int(x));
		FIXED mapY = int2fx(fx2int(y));

		FIXED sideDistX;
		FIXED sideDistY;

		FIXED deltaDistX; 
		FIXED deltaDistY;
		if (rayDirX == 0) {
			deltaDistX = int2fx(1024);
		}
		else {
			deltaDistX = fixedAbs(fxdiv(int2fx(1), rayDirX));
		}

		if (rayDirY == 0) {
			deltaDistY = int2fx(1024);
		}
		else {
			deltaDistY = fixedAbs(fxdiv(int2fx(1), rayDirY));
		}


		FIXED stepX;
		FIXED stepY;

		int side;

		if(rayDirX < 0) {
			stepX = int2fx(-1);
			sideDistX = fxmul(fxsub(x, mapX) , deltaDistX);
		}
		else {
			stepX = int2fx(1);
			sideDistX = fxmul(fxsub(fxadd(mapX, int2fx(1.0)), x), deltaDistX);
		}
		if(rayDirY < 0) {
			stepY = int2fx(-1);
			sideDistY = fxmul(fxsub(y, mapY), deltaDistY);
		}
		else {
			stepY = int2fx(1);
			sideDistY = fxmul(fxsub(fxadd(mapY, int2fx(1.0)), y), deltaDistY);
		}

	
		int hit = 0;
		int xCell;
		int yCell;
		for (int j = 0; j < 100; j++) {
			if(sideDistX < sideDistY) {
				sideDistX = fxadd(sideDistX, deltaDistX);
				mapX = fxadd(mapX, stepX);
				side = 0;
			}
			else {
				sideDistY = fxadd(sideDistY, deltaDistY);
				mapY = fxadd(mapY, stepY);
				side = 1;
			}

			int currentXCell = fx2int(mapX);
			int currentYCell = fx2int(mapY); 
        	if(MAP[currentXCell*MAPSIZE + currentYCell] > 0) {
				xCell = currentXCell;
				yCell = currentYCell;
				hit = 1;
				break;
			}


		}

		FIXED perpWallDistance;
		int textureColumn;
		FIXED wallPos;

		if (hit) {
			//horizontal
			if(side == 0) {
				perpWallDistance = fxsub(sideDistX, deltaDistX);
				wallPos = fxadd(y, fxmul(perpWallDistance, rayDirY));
			}
			//vertical
			else {
				perpWallDistance = fxsub(sideDistY, deltaDistY);
				wallPos = fxadd(x, fxmul(perpWallDistance, rayDirX));
			}          

			wallPos = fxsub(wallPos, int2fx(fx2int(wallPos)));
			textureColumn = fx2int(fxmul(wallPos, int2fx(TEXTURESIZE)));


			if(side == 0 && rayDirX < 0) textureColumn = TEXTURESIZE - textureColumn - 1;
			if(side == 1 && rayDirY > 0) textureColumn = TEXTURESIZE - textureColumn - 1;


			zBuffer[i] = perpWallDistance;

			drawWall(2*i, perpWallDistance, MAP[xCell*MAPSIZE + yCell], side, textureColumn);

		}
		else {
			m4_dual_vline(2*i, 0, 160, 0);
		}
	}
}

void castForward() {
	if (player.hasKey) {
		int distance = castRay(4);
		if (distance >= 0 && distance < 2) {
			initLevel();
		}
	}
}

void fire() {
	for(int i = 0; i < MAXENTITYCOUNT; i++) {
		if (entities[i].active) {
			continue;
		}
		
		entities[i].x = x;
		entities[i].y = y;
		entities[i].texture = 5;
		entities[i].type = 2;
		entities[i].active = true;
		entities[i].scale = 128;
		entities[i].xDir = dirX;
		entities[i].yDir = dirY;
		entities[i].moving = true;
		entities[i].yOffset = 256;



		break;
	}
}

void moveEntities() {
	for(int i = 0; i < MAXENTITYCOUNT; i++) {
		if (!entities[i].active || !entities[i].moving) {
			continue;
		}
		if (entities[i].type == 2) { //projectile
			entities[i].x = fxadd(entities[i].x, entities[i].xDir);
			entities[i].y = fxadd(entities[i].y, entities[i].yDir);
		}

	}
}


int compareDistances(const void *v1, const void *v2)
{
    const struct Entity *u1 = v1;
    const struct Entity *u2 = v2;
    return u1->distance < u2->distance;
}

// function to swap elements
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}




// function to find the partition position
int partition(int array[], int low, int high) {
  
  // select the rightmost element as pivot
  int pivot = entities[array[high]].distance;
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (entities[array[j]].distance >= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}


void quickSort(int array[], int low, int high) {
  if (low < high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);
    
    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}

//sort algorithm
//sort the sprites based on distance
void sortEntities()
{

	quickSort(entityOrder, 0, MAXENTITYCOUNT - 1);

}



int drawEntities() {

	//update distances to player 
	for(int i = 0; i < MAXENTITYCOUNT; i++) {
	  if (!entities[i].active) {
		entities[i].distance = int2fx(1024);
	  }
	  else {
      	entities[i].distance = fxmul(fxsub(x, entities[i].x), fxsub(x, entities[i].x)) + fxmul(fxsub(y, entities[i].y), fxsub(y, entities[i].y));
	  }
    }

    sortEntities();

	for(int i = 0; i < MAXENTITYCOUNT; i++) {
		if (!entities[entityOrder[i]].active || entities[entityOrder[i]].distance < 64) {
			continue;
		}
		FIXED entityX = fxsub(entities[entityOrder[i]].x, x);
		FIXED entityY = fxsub(entities[entityOrder[i]].y, y);

		FIXED invDet = fxdiv(int2fx(1), fxsub(fxmul(planeX, dirY), fxmul(dirX, planeY)));
		FIXED transformX = fxmul(invDet , fxsub(fxmul(dirY, entityX), fxmul(dirX, entityY)));
		FIXED transformY = fxmul(invDet , fxadd(fxmul(-planeY, entityX),  fxmul(planeX, entityY)));

		if (transformX == 0) {
			transformX = 16;
		}
		if (transformY == 0) {
			transformY = 16;
		}


		int spriteScreenX = fx2int(fxmul(int2fx(SCREENWIDTH >> 1), (fxadd(int2fx(1), fxdiv(transformX, transformY)))));


		//calculate height of the sprite on screen
		int spriteHeight = fixedAbs(fx2int(fxdiv(fxmul(int2fx(SCREENHEIGHT), entities[entityOrder[i]].scale), (transformY)))); //using 'transformY' instead of the real distance prevents fisheye
		int offsetY = fx2int(fxmul(int2fx(spriteHeight) , entities[entityOrder[i]].yOffset));
		//calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -spriteHeight / 2 + SCREENHEIGHT / 2 + offsetY;
		if(drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + SCREENHEIGHT / 2 + offsetY;
		if(drawEndY >= SCREENHEIGHT) drawEndY = SCREENHEIGHT - 1;


		//calculate width of the sprite
		int spriteWidth = spriteHeight;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		drawStartX /= 2;
		drawEndX /= 2;


		FIXED horizontalTexFrag = fxdiv(int2fx(2*TEXTURESIZE), int2fx(spriteWidth));
		//keeps track of which column of the texture should be drawn, (FIXED point fractional)
		//starting from zero leads to artifacting on the very first vertical stripe
		//instead first column set to 32/256 => 0.125
		FIXED hTexPos = 32;
		if (transformY > 0) {
			for(int stripe = drawStartX; stripe < drawEndX; stripe++) {
				if (stripe >= 0 && stripe < SCREENWIDTH/2 ) {
					if(transformY < zBuffer[stripe]) {
						int texX = fx2int(hTexPos);
						int texture = entities[entityOrder[i]].texture;
						m4_sprite_textured_dual_line(2*stripe, drawStartY, drawEndY, drawEndY-drawStartY, texture , texX);
					}
				}
				else if (stripe > SCREENWIDTH/2) {
					break;
				}
				hTexPos = fxadd(hTexPos, horizontalTexFrag);
			}
		}



	}
}

void removeEntity(int i) {
	entities[i].active = false;
}

void checkEntityCollisions() {
	for(int i = 0; i < MAXENTITYCOUNT; i++) {
		if (!entities[i].active) {
			continue;
		}

		//check for projectile collisions with walls
		if (entities[i].moving) {
			int xCell = fx2int(entities[i].x);
			int yCell = fx2int(entities[i].y);
			if (MAP[xCell*MAPSIZE + yCell] != 0) {
				removeEntity(i);
			}

		}

		//check for key pickup by player
		int type = entities[i].type;
		if (type == 1) {
			if (entities[i].distance < 64) {
				removeEntity(i);
				player.hasKey = true;
			}
		}

	}
}


void updateDirection() {

	FIXED viewPlaneMultiplier = 200;//168;

	const FIXED PI2 = int2fx(0x10000 >> 1);
	FIXED luAngle = fxmul(PI2, fxdiv(direction, int2fx(360))) >> 7;

	FIXED cosine = lu_cos(luAngle);
	FIXED sine = lu_sin(luAngle);

	dirX = (fxdiv(cosine, int2fx(16)));
	dirY = (-fxdiv(sine, int2fx(16)));

	planeY = (fxdiv(fxmul(cosine, viewPlaneMultiplier) ,int2fx(16)));
	planeX = (fxdiv(fxmul(sine, viewPlaneMultiplier) ,int2fx(16)));

	//return planeX * 100.0;//(fxmul(dirX, int2fx(100)));



}

bool collisionCheck(FIXED x, FIXED y) {
	if (MAP[fx2int(x) * MAPSIZE + fx2int(y)] != 0) {
		return true;
	}
	else {
		return false;
	}
}

void move(int type) {
	const FIXED SPEED = float2fx(0.1); 
	FIXED moveX = 0;
	FIXED moveY = 0;

	if (type == 0) { //left1
		moveX = fxmul(dirY, SPEED);
		moveY = -fxmul(dirX, SPEED);
	}
	else if (type == 1) { //right
		moveX = -fxmul(dirY, SPEED);
		moveY = fxmul(dirX, SPEED);
	}
	else if (type == 2) { //forward
		moveX = fxmul(dirX, SPEED);
		moveY = fxmul(dirY, SPEED);
	}
	else { //back
		moveX = -fxmul(dirX, SPEED);
		moveY = -fxmul(dirY, SPEED);
	}

	if (!collisionCheck(fxadd(x, fxadd(moveX, moveX)), y)) {
		x = fxadd(x, moveX);
	}
	if (!collisionCheck(x, fxadd(y, fxadd(moveY, moveY)))) {
		y = fxadd(y, moveY);
	}


}

int main(void)
{



	
	
	REG_DISPCNT= DCNT_MODE4 | DCNT_BG2;
	initCameraXLu();
	initTextureStepLu();
	initPalette();
	initHud();
	vid_flip(); 
	initHud();


	initLevel();
		
	/*
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	updateDirection();

	irq_init(NULL);
	irq_enable(II_VBLANK);

	tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
	tte_set_pos(92, 68);
	castRays();
	FIXED test = drawSprites();


	char str[8];
	sprintf(str, "%d", test); //65536
	tte_write(str);
	
	*/
	


	
	while (1) {

		
		key_poll();
		if (key_is_down(KEY_LEFT)) {
			move(0);
		}
		else if (key_is_down(KEY_RIGHT)) {
			move(1);
		}
		if (key_is_down(KEY_UP)) {
			move(2);
		}
		else if (key_is_down(KEY_DOWN)) {
			move(3);
		}
		if (key_is_down(KEY_R)) {
			direction -= int2fx(5);
		}
		else if (key_is_down(KEY_L)) {
			direction += int2fx(5);
		}
		if (fx2int(direction) >= 360) {
			direction = fxsub(direction, int2fx(360));
		}
		if (fx2int(direction) < 0) {
			direction = fxadd(direction, int2fx(360));
		}

		if (key_hit(KEY_A)) {
			castForward();
		}
		if (key_hit(KEY_B)) {
			fire();
		}

 		updateDirection();
		//x = fxadd(x, 1);
		//y = fxadd(y, 4);

		castRays();
		moveEntities();
		drawEntities();
		checkEntityCollisions();
		vid_flip(); 
		
		
		
		//VBlankIntrWait();
	}
}
