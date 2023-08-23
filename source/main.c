#include <tonc.h>
#include <math.h>

const float PI = 3.1415;
int MAP[8][8] = {0};
const int SCREENHEIGHT = 160;
const int SCREENWIDTH = 240;
const int CASTEDRAYS = 240;
const int FOV = 60;
const int TILESIZE = 8;
const int MAXDEPTH = TILESIZE * 10;
const int DISTANCETOVIEWPORT = 255; //half of width / tan(half of fov)

void initMap() {

	MAP[4][3] = 2;
	//MAP[4][4] = 1;
	MAP[3][3] = 1;
	MAP[5][3] = 3;

	for(int i = 0; i < 8; i++) {
		MAP[0][i] = 1;
		MAP[8][i] = 1;
		MAP[i][0] = 3;
		MAP[i][8] = 1;
	}
}

void castRays(FIXED x, FIXED y, int direction) {
	const FIXED PI2 = int2fx(0x10000 >> 1);//float2fx(2*PI);

	const FIXED STEPANGLE = fxdiv(int2fx(FOV), int2fx(CASTEDRAYS));
	int positiveAngle = direction - FOV/2;
	if (positiveAngle < 0) {
		positiveAngle = 360 + positiveAngle;
	}
	FIXED angle = int2fx(positiveAngle % 360);
	const FIXED FIXEDTILESIZE = int2fx(TILESIZE);

	for (int i = 0; i < CASTEDRAYS; i++) {
		for (int depth = 0; depth < MAXDEPTH; depth++) {

			int luAngle = 0;
			if (angle != 0) {
				luAngle = fxmul(PI2, fxdiv(angle, int2fx(360))) >> 7;

			}

			FIXED moveX = fxdiv(fxmul(lu_sin(luAngle), int2fx(depth)), int2fx(16));
            FIXED targetX = fxsub(fxmul(x, int2fx(TILESIZE)), moveX);
            FIXED tileX = fxdiv(targetX, FIXEDTILESIZE); 
			
			FIXED luCos = lu_cos(luAngle);
			FIXED moveY = fxdiv(fxmul(luCos, int2fx(depth)), int2fx(16));
            FIXED targetY = fxsub(fxmul(y, int2fx(TILESIZE)), moveY);
            FIXED tileY = fxdiv(targetY, FIXEDTILESIZE); 

			int tileXi = fx2int(tileX);
			int tileYi = fx2int(tileY);
			CLAMP(tileXi, 0, 7);
			CLAMP(tileYi, 0, 7);

			if (MAP[tileYi][tileXi] != 0)
			{
				int wallHeight = 160;
				if (depth != 0) {
					wallHeight = 160-6*depth;
					wallHeight = CLAMP(TILESIZE * DISTANCETOVIEWPORT / (depth), 0, 160);

				}
				int halfHeight = wallHeight >> 1;

				int brightness = 15;
				/*
				if ( targetX - fxmul(tileX, FIXEDTILESIZE) == 0) {
					brightness *= 0.5;
				}
				*/

				if (MAP[tileYi][tileXi] == 1) {
					m3_line(i, 0, i, 80-halfHeight, RGB15(16,16,16));
					m3_line(i, 80-halfHeight, i, 80+halfHeight, RGB15(brightness,0,0));
					m3_line(i, 80+halfHeight, i, 160, RGB15(5,5,5));
					break;
				}					
				if (MAP[tileYi][tileXi] == 2) {
					m3_line(i, 0, i, 80-halfHeight, RGB15(16,16,16));
					m3_line(i, 80-halfHeight, i, 80+halfHeight, RGB15(0,brightness,0));
					m3_line(i, 80+halfHeight, i, 160, RGB15(5,5,5));
					break;
				}
				if (MAP[tileYi][tileXi] == 3) {
					m3_line(i, 0, i, 80-halfHeight, RGB15(16,16,16));
					m3_line(i, 80-halfHeight, i, 80+halfHeight, RGB15(0,0,brightness));
					m3_line(i, 80+halfHeight, i, 160, RGB15(5,5,5));
					break;
				}

			}

			
			if (depth +1 == MAXDEPTH) {
				m3_line(i, 0, i, 160, RGB15(0,0,0));

			}
			
			
		}
		angle = fxadd(angle, STEPANGLE);


	}
}

int main(void)
{

	FIXED x = float2fx(1.75);
	FIXED y = float2fx(1.1);

	int direction = 180;

	initMap();
	
	
	REG_DISPCNT= DCNT_MODE3 | DCNT_BG2;

    // Fill screen with grey color
	

	/*
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	irq_init(NULL);
	irq_enable(II_VBLANK);

	tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
	tte_set_pos(92, 68);

	FIXED targetX = castRays(x, y, direction);

	char str[8];
	sprintf(str, "%d", fx2int(targetX)); //65536
	tte_write(str);
	*/
	
	
	while (1) {
		castRays(x, y, direction);
		direction += 0;
		//x = fxadd(x, float2fx(0.1));
		//y = fxadd(y, float2fx(0.1));
		if (direction > 360) {
			direction = 0;
		}
		if (direction < 0) {
			direction = 360;
		}

		//VBlankIntrWait();
	}
}
