
#include <tonc.h>

#define TILESIZE 1
#define SCREENHEIGHT 160
#define SCREENWIDTH 240
#define FOV 60


const float PI = 3.1415;
const int MAPSIZE = 8;

int MAP[8][8] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 3, 0, 3, 0, 0, 1},
        {1, 0, 3, 0, 3, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}
    };

FIXED CAMERAX_LU[SCREENWIDTH / 2] = {0};

const int CASTEDRAYS = SCREEN_WIDTH / 2;
const int HALFFOV = FOV / 2;
const int TILESHIFTER = log2(TILESIZE); 
const FIXED FIXEDTILESIZE = TILESIZE * 256; // equal to int2fx(TILESIZE);

FIXED x, y;
FIXED dirX, dirY;
FIXED planeX, planeY;
int direction;



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


void loadColorToPalette(COLOR c) {
	palette[nextPaletteIndex] = c;
	nextPaletteIndex++;
}

void initPalette() {
	loadColorToPalette(RGB15(0,0,0));
	loadColorToPalette(RGB15(3,3,3));
	loadColorToPalette(RGB15(8,8,8));
	loadColorToPalette(RGB15(16,16,16));
	loadColorToPalette(RGB15(8,0,0));
	loadColorToPalette(RGB15(16,0,0));
	loadColorToPalette(RGB15(0,8,0));
	loadColorToPalette(RGB15(0,16,0));
	loadColorToPalette(RGB15(0,0,8));
	loadColorToPalette(RGB15(0,0,16));
	loadColorToPalette(RGB15(8,8,0));
	loadColorToPalette(RGB15(16,16,0));


}

void initCameraXLu() {
	for (int i = 0; i < CASTEDRAYS; i++) {
		CAMERAX_LU[i] = fxsub(fxdiv(int2fx(2 * i) ,int2fx(CASTEDRAYS )) , int2fx(1));

	}
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


void drawWall(int i, float distance, int type, int vertical) {

	int wallHeight = 160/distance;
	wallHeight = CLAMP(wallHeight, 1, 160);
	int halfHeight = (wallHeight >> 1);

	m4_dual_vline(i, 0, 80-halfHeight, 2);

	int color = 0;
	
	if (type == 1) {
		color = 4;
	}
	else if (type == 2) {
		color = 6;
	}
	else if (type == 3) {
		color = 8;
	}
	else if (type == 4) {
		color = 10;
	}
	if (!vertical) {
		color++;
	}
	//the actual wall
	m4_dual_vline(i, 80-halfHeight, 80 + halfHeight, color);

	m4_dual_vline(i, 80+ halfHeight, 160, 1);


}

int castGrid() {
	//pi2 equivalent for calling lu_sin & cos
	//must be scaled down to avoid overflow later


	for (int i= 0; i < CASTEDRAYS; i++) {


		const FIXED cameraX = CAMERAX_LU[i]; //x-coordinate in camera space
		const FIXED rayDirX = fxadd(dirX, fxmul(planeX, cameraX));
    	const FIXED rayDirY = fxadd(dirY, fxmul(planeY, cameraX));
		
		
		FIXED mapX = int2fx(fx2int(x));
		FIXED mapY = int2fx(fx2int(y));
		//int2fx(fx2int(y));

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

		float perpWallDistance;

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
        	if(MAP[currentXCell][currentYCell] > 0) {
				xCell = currentXCell;
				yCell = currentYCell;
				hit = 1;
				break;
			}


		}

		if (hit) {
			if(side == 0) perpWallDistance = fx2float(fxsub(sideDistX, deltaDistX));
			else          perpWallDistance = fx2float(fxsub(sideDistY, deltaDistY));

			drawWall(2*i, perpWallDistance, MAP[xCell][yCell], side);

		}
		else {
			m4_dual_vline(2*i, 0, 160, 0);
		}









	}


}


int updateDirection() {

	FIXED viewPlaneMultiplier = 168;

	const FIXED PI2 = int2fx(0x10000 >> 1);
	FIXED luAngle = fxmul64(PI2, fxdiv(direction, int2fx(360))) >> 7;

	FIXED cosine = lu_cos(luAngle);
	FIXED sine = lu_sin(luAngle);

	dirX = (fxdiv(cosine, int2fx(16)));
	dirY = (-fxdiv(sine, int2fx(16)));

	planeY = (fxdiv(fxmul(cosine, viewPlaneMultiplier) ,int2fx(16)));
	planeX = (fxdiv(fxmul(sine, viewPlaneMultiplier) ,int2fx(16)));

	//return planeX * 100.0;//(fxmul(dirX, int2fx(100)));



}

bool collisionCheck(FIXED x, FIXED y) {
	if (MAP[fx2int(x)][fx2int(y)] != 0) {
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


	x = int2fx(2);//96;//2*64;//
	y = int2fx(2);//224;//2*64;//

	direction = 0;

	
	
	REG_DISPCNT= DCNT_MODE4 | DCNT_BG2;
	initCameraXLu();
	initPalette();

		
	/*
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	irq_init(NULL);
	irq_enable(II_VBLANK);

	tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
	tte_set_pos(92, 68);

	FIXED test = updateDirection();


	char str[8];
	sprintf(str, "%d", test); //65536
	tte_write(str);
	*/
	
	


	
	while (1) {

		
		key_poll();
		if (key_held(KEY_LEFT)) {
			move(0);
		}
		else if (key_held(KEY_RIGHT)) {
			move(1);
		}
		if (key_held(KEY_UP)) {
			move(2);
		}
		else if (key_held(KEY_DOWN)) {
			move(3);
		}
		if (key_held(KEY_R)) {
			direction -= int2fx(3);
		}
		else if (key_held(KEY_L)) {
			direction += int2fx(3);
		}
		if (fx2int(direction) >= 360) {
			direction = 0;
		}
		if (fx2int(direction) < 0) {
			direction += int2fx(360);
		}
 		updateDirection();
		//x = fxadd(x, 1);
		//y = fxadd(y, 4);

		castGrid();
		vid_flip(); 
		
		
		
		//VBlankIntrWait();
	}
}
