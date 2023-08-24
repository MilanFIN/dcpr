#include <tonc.h>

const float PI = 3.1415;
const int MAPSIZE = 5;
int MAP[5][5] = {0};
const int SCREENHEIGHT = 160;
const int SCREENWIDTH = 240;
const int CASTEDRAYS = 240;
const int FOV = 60;
const int TILESIZE = 64; //8
const int MAXDEPTH = TILESIZE * 10;
const int DISTANCETOVIEWPORT = 255; //half of width / tan(half of fov)

void initMap() {

	/*
	MAP[4][3] = 2;
	MAP[3][3] = 1;
	MAP[5][3] = 3;
	*/

	for(int i = 0; i < MAPSIZE; i++) {
		MAP[0][i] = 1;
		MAP[MAPSIZE-1][i] = 2;
		MAP[i][0] = 3;
		MAP[i][MAPSIZE-1] = 4;
		for (int j = 0; j < 8; j++) {
			//MAP[i][j] = 1;
		}
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


int castGrid2(FIXED x, FIXED y, int direction) {
	const FIXED PI2 = int2fx(0x10000 >> 1);

	const FIXED STEPANGLE = fxdiv64(int2fx(FOV), int2fx(CASTEDRAYS));
	//TODO REVERT BACK
	int positiveAngle = direction - FOV/2;
	if (positiveAngle < 0) {
		positiveAngle = 360 + positiveAngle;
	}
	if (positiveAngle < 0) {
		return;
	}
	FIXED angle = int2fx(positiveAngle % 360);
	const FIXED FIXEDTILESIZE = int2fx(TILESIZE);

	//input scaling
	y = fxmul64(y, FIXEDTILESIZE);
	x = fxmul64(x, FIXEDTILESIZE);

	for (int i = 0; i < CASTEDRAYS; i++) {
		int luAngle = fxmul64(PI2, fxdiv64(angle, int2fx(360))) >> 7;
		int rayAngle = fx2int(angle);
		FIXED sine = fxdiv64(lu_sin(luAngle), 16);
		FIXED cosine = fxdiv64(lu_cos(luAngle), 16);
		FIXED tan = fxdiv64(sine, cosine);


		//initial position that will be checked for a wall
		FIXED aY, aX;
		//the length of steps that will be taken afterwards for each raymarch
		FIXED yA, xA;
		if (rayAngle >= 0 && rayAngle < 180) {
			aY = fxsub(fxmul64(fxdiv64(y, FIXEDTILESIZE), FIXEDTILESIZE), 1);
			yA = fxmul64(FIXEDTILESIZE, int2fx(-1));
		}
		else {
			aY = fxadd(fxmul64(fxdiv64(y, FIXEDTILESIZE), FIXEDTILESIZE), FIXEDTILESIZE);
			yA = FIXEDTILESIZE;
		}
		FIXED division;

		if (tan != 0) {
			xA = fxdiv64(FIXEDTILESIZE, tan);
			division = fxdiv64(fxsub(y, aY), tan);
		}
		else {
			xA = FIXEDTILESIZE;
			division = 0;
		}
		aX = fxadd(x, division);


		int initX = fx2int(fxdiv64(aX, FIXEDTILESIZE));
		int initY = fx2int(fxdiv64(aY, FIXEDTILESIZE));
		
		initX = CLAMP(initX, 0,MAPSIZE-1);
		initY = CLAMP(initY, 0, MAPSIZE-1);

		if (MAP[initX][initY] != 0) {
			drawWall(i, y, aY, sine, MAP[initX][initY]);
		}
		
		else {
			int iterations = 1;
			while (true) {
				aX = fxadd(aX, xA);
				aY = fxadd(aY, yA);
				int nextX = fx2int(fxdiv64(aX, FIXEDTILESIZE));
				int nextY = fx2int(fxdiv64(aY, FIXEDTILESIZE));
				nextX = CLAMP(nextX, 0,MAPSIZE-1);
				nextY = CLAMP(nextY, 0, MAPSIZE-1);
				
				if (MAP[nextX][nextY] != 0) {

					drawWall(i, y, aY, sine, MAP[nextX][nextY]);
					//m3_line(i, iterations*4, i, 160, RGB15(0,0,16));
					break;
				}
				iterations += 1;
				if (iterations > 20) {
					break;
				}
				
				
			}
		}
		
		

		//return MAP[initX][initY];

		angle = fxadd(angle, STEPANGLE);
		int intAngle = fx2int(angle) % 360;
		if (intAngle < 0) {
			intAngle = 360+intAngle;
			angle = int2fx(intAngle);
		}


	}
}

void drawWall(int i, int y, int aY, float sine, int type) {

	int distance = (aY - y) / sine;
	
	if (distance < 0) {
		distance = -distance;
	}
	int wallHeight = 160;// / distance;
	int halfHeight = wallHeight / 4;
	//m3_line(i, 0, i, 80-halfHeight, RGB15(0,0,0));

	if (type == 1) {
		m3_line(i, 80-halfHeight, i, 80 + halfHeight, RGB15(0,0,16));
	}
	if (type == 2) {
		m3_line(i, 80-halfHeight, i, 80 + halfHeight, RGB15(0,16,0));
	}
	if (type == 3) {
		m3_line(i, 80-halfHeight, i, 80 + halfHeight, RGB15(16,16,0));
	}
	if (type == 4) {
		m3_line(i, 80-halfHeight, i, 80 + halfHeight, RGB15(16,0,0));
	}
	//m3_line(i, 80+ halfHeight, i, 160, RGB15(0,0,0));


}

int castGrid(int x, int y, int direction) {
	const FIXED PI2 = int2fx(0x10000 >> 1);

	const FIXED STEPANGLE = fxdiv64(int2fx(FOV), int2fx(CASTEDRAYS));

	int positiveAngle = direction;// - FOV/2;
	if (positiveAngle < 0) {
		positiveAngle = 360 + positiveAngle;
	}
	FIXED angle = int2fx(positiveAngle % 360);

	for (int i = 0; i < CASTEDRAYS; i++) {

		int luAngle = fxmul64(PI2, fxdiv64(angle, int2fx(360))) >> 7;
		if (fx2int(luAngle) < 0) {
			continue;
		}
		int rayAngle = fx2int(angle);

		//ugly floating point math, placeholder until 
		float sine = fx2float(fxdiv(lu_sin(luAngle), 16));
		float cosine = fx2float(fxdiv(lu_cos(luAngle), 16));
		int tan = 64*sine/cosine; // in 1/64th range, so 64 equals one
		//avoid division by zero later on
		if (tan == 0) {
			tan = 1;
		}


		/*
		this part checks for horizontal walls on the 2d map
		*/
		
		//initial coordinate to check for horizontal walls
		int tx, ty;
		//lengths of further steps that will be taken for checking collisions
		int dx, dy;
		if (rayAngle >= 0 && rayAngle < 180) {
			ty = y / TILESIZE * TILESIZE -1;
			dy = -TILESIZE;
		}
		else {
			ty = y / TILESIZE * TILESIZE + TILESIZE;
			dy = TILESIZE;
		}
		tx = x + (y-ty)*64/tan; //64 to correct for tan scaling
   		dx = TILESIZE*64/tan; //64 to correct for tan scaling

		int initX = tx / TILESIZE;
		int initY = ty / TILESIZE;
		
		if (initX >= 0 && initX < MAPSIZE && initY >= 0 && initY < MAPSIZE) {

			if (MAP[initX][initY] != 0) {
				drawWall(i, y, ty, sine, MAP[initX][initY]);
			}
			else {
				//step tx & ty up in steps of dx & dy 
				for (int j = 0; j < 20; j++) {
					tx += dx;
					ty += dy;
					int nextX = tx / TILESIZE;
					int nextY = ty / TILESIZE;
					if (nextX < 0 || nextX >= MAPSIZE || nextY < 0 || nextY >= MAPSIZE) {
						break;
					}
					if (MAP[nextX][nextY] != 0) {
						drawWall(i, y, ty, sine, MAP[nextX][nextY]);
						break;
					}
				}

			}
			

		}


		/*
		this part checks for vertical walls on the 2d map
		*/
		/*
		//initial coordinate to check for horizontal walls
		int txh, tyh;
		//lengths of further steps that will be taken for checking collisions
		int dxh, dyh;

		if (rayAngle >= 90 && rayAngle < 270) {
			txh = x / TILESIZE * TILESIZE -1;
			dxh = -TILESIZE;
		}
		else {
			txh = x / TILESIZE * TILESIZE + TILESIZE;
			dxh = TILESIZE;
		}
		tyh = y + (x-txh)*tan/64; //64 to correct for tan scaling
   		dyh = TILESIZE*tan/64; //64 to correct for tan scaling


		int initXH = txh / TILESIZE;
		int initYH = tyh / TILESIZE;
		if (initXH >= 0 && initXH < MAPSIZE && initYH >= 0 && initYH < MAPSIZE) {
			if (MAP[initXH][initYH] != 0) {
				drawWall(i, y, tyh, sine, MAP[initXH][initYH]);
			}
			
			else {
				//step tx & ty up in steps of dx & dy 
				for (int j = 0; j < 20; j++) {
					txh += dxh;
					tyh += dyh;
					int nextX = txh / TILESIZE;
					int nextY = tyh / TILESIZE;
					if (nextX < 0 || nextX >= MAPSIZE || nextY < 0 || nextY >= MAPSIZE) {
						break;
					}
					if (MAP[nextX][nextY] != 0) {
						drawWall(i, y, tyh, sine, MAP[nextX][nextY]);
						break;
					}
				}

			}
		}
		*/


		





		angle = fxadd(angle, STEPANGLE);
		if (fx2int(angle) > 360) {
			angle = fxsub(angle, int2fx(360));
		}
	}

}


int main(void)
{

	//FIXED x = float2fx(3);
	//FIXED y = float2fx(3);

	int x = 96;//2*64;//
	int y = 224;//2*64;//

	int direction = 0; //315, 0,0

	initMap();
	
	
	REG_DISPCNT= DCNT_MODE3 | DCNT_BG2;
		

	/*
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	irq_init(NULL);
	irq_enable(II_VBLANK);

	tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
	tte_set_pos(92, 68);

	int test = castGrid(x, y, direction);

	char str[8];
	sprintf(str, "%d", test); //65536
	tte_write(str);
	
	*/
	
	
	while (1) {
		
		
		castGrid(x, y, direction);
		direction += 1;
		//x = fxadd(x, float2fx(0.1));
		//y = fxadd(y, float2fx(0.1));
		if (direction >= 360) {
			direction = 0;
		}
		
		
		
		

		//VBlankIntrWait();
	}
}
