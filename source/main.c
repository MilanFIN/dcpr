#include <tonc.h>

#define TILESIZE 1
#define SCREENHEIGHT 160
#define SCREENWIDTH 240
#define FOV 60


const float PI = 3.1415;
const int MAPSIZE = 8;

int MAP[8][8] = {
        {1, 2, 1, 2, 1, 2, 2, 1},
        {2, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 2},
        {1, 3, 1, 3, 1, 3, 3, 3}
    };


const int CASTEDRAYS = SCREEN_WIDTH / 2;
const int HALFFOV = FOV / 2;
const int TILESHIFTER = log2(TILESIZE); 
const FIXED FIXEDTILESIZE = TILESIZE * 256; // equal to int2fx(TILESIZE);
const FIXED SPEED = TILESIZE; //equal to tilesize results in relatively smooth movement

float x, y;
float dirX, dirY;
float planeX, planeY;




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

	int wallHeight = 160*TILESIZE/distance;//fx2int(fxdiv(int2fx(TILESIZE >> 2), distance) << 5) ;
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


		float cameraX = 2 * i / (float) CASTEDRAYS - 1; //x-coordinate in camera space
		float rayDirX = (dirX + (planeX * cameraX));
    	float rayDirY = (dirY + (planeY * cameraX));
		
		
		int mapX = x;
		int mapY = y;

		float sideDistX;
		float sideDistY;

		float deltaDistX; 
		float deltaDistY;
		if (rayDirX == 0) {
			deltaDistX = (1e30);
		}
		else {
			deltaDistX = floatAbs((TILESIZE) / rayDirX);
		}

		if (rayDirY == 0) {
			deltaDistY = (1e30);
		}
		else {
			deltaDistY = floatAbs((TILESIZE) / rayDirY);
		}

		float perpWallDistance;

		float stepX;
		float stepY;

		int side;

		if(rayDirX < 0) {
			stepX = (-TILESIZE);
			sideDistX = (x - mapX) * deltaDistX;
		}
		else {
			stepX = (TILESIZE);
			sideDistX = ((mapX + 1.0) - x) * deltaDistX;
		}
		if(rayDirY < 0) {
			stepY = -TILESIZE;
			sideDistY = (y - mapY) * deltaDistY;
		}
		else {
			stepY = TILESIZE;
			sideDistY = ((mapY + 1.0) - y) * deltaDistY;
		}

	
		int hit = 0;
		int xCell;
		int yCell;
		for (int j = 0; j < 100; j++) {
			if(sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}

			int currentXCell = (mapX) / TILESIZE;
			int currentYCell = (mapY) / TILESIZE;
        	if(MAP[currentXCell][currentYCell] > 0) {
				xCell = currentXCell;
				yCell = currentYCell;
				hit = 1;
				break;
			}


		}

		if (hit) {
			if(side == 0) perpWallDistance = (sideDistX - deltaDistX);
			else          perpWallDistance = (sideDistY - deltaDistY);

			drawWall(2*i, perpWallDistance, MAP[xCell][yCell], side);

		}
		else {
			m4_dual_vline(2*i, 0, 160, 0);
		}









	}


}

int main(void)
{



	x = 4*TILESIZE;//96;//2*64;//
	y = 4*TILESIZE;//224;//2*64;//

	dirX = 1;
	dirY = 0;

	planeX = 0;
	planeY = 0.66;


	int rotSpeed = 1;

	
	
	REG_DISPCNT= DCNT_MODE4 | DCNT_BG2;
	initPalette();

		
	/*
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	irq_init(NULL);
	irq_enable(II_VBLANK);

	tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
	tte_set_pos(92, 68);

	FIXED test = castGrid();


	char str[8];
	sprintf(str, "%d", test); //65536
	tte_write(str);
	*/
	
	


	
	while (1) {
		/*
		if (dirX != 0) {
			dirX = 0;
			dirY = -1;
			planeX = 0.66;
			planeY = 0;
		}
		else {
			dirX = 1;
			dirY = 0;
			planeX = 0;
			planeY = 0.66;
		}
		*/
			
		
		castGrid();
		
		vid_flip(); 
		
		
		
		//VBlankIntrWait();
	}
}
