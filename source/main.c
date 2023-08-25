#include <tonc.h>

const float PI = 3.1415;
const int MAPSIZE = 8;
int MAP[8][8] = {0};
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

	//MAP[2][2] = 1;

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

float floatAbs(float a) {
	if (a < 0.0) {
		return -a;
	}
	else {
		return a;
	}
}

FIXED fixedAbs(FIXED a) {
	if (fx2int(a) < 0) {
		return fxsub(int2fx(0), a);
	}
	else {
		return a;
	}
}


void drawWall(int i, FIXED distance, int type) {

	int wallHeight = fx2int(fxmul(fxdiv(int2fx(32), distance), int2fx(10)));//160;// / distance;
	wallHeight = CLAMP(wallHeight, 1, 160);
	int halfHeight = wallHeight / 2;
	m3_line(i, 0, i, 80-halfHeight, RGB15(8,8,8));

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
	m3_line(i, 80+ halfHeight, i, 160, RGB15(3,3,3));


}

int castGrid(int x, int y, int direction) {
	const FIXED PI2 = int2fx(0x10000 >> 1);

	const FIXED STEPANGLE = fxdiv64(int2fx(FOV), int2fx(CASTEDRAYS));

	int positiveAngle = direction;// - FOV/2;
	if (positiveAngle < 0) {
		positiveAngle = 360 + positiveAngle;
	}
	FIXED angle = int2fx(positiveAngle % 360);
	const FIXED FIXEDTILESIZE = int2fx(TILESIZE);
	
	const FIXED fixedY = int2fx(y);
	const FIXED fixedX = int2fx(x);

	for (int i = 0; i < CASTEDRAYS; i++) {

		int luAngle = fxmul64(PI2, fxdiv64(angle, int2fx(360))) >> 7;
		if (fx2int(luAngle) < 0) {
			continue;
		}
		int rayAngle = fx2int(angle);

		int xDir, yDir;
		if (rayAngle >= 0 && rayAngle < 90) {
			xDir = 1;
			yDir = 1;
		}
		else if (rayAngle >= 90 && rayAngle < 180) {
			xDir = 1;
			yDir = 1;
		}
		else if (rayAngle >= 180 && rayAngle < 270) {
			xDir = -1;
			yDir = 1;
		}
		else if (rayAngle >= 270) {
			xDir = -1;
			yDir = -1;
		}



		//ugly floating point math, placeholder until 
		FIXED sine = lu_sin(luAngle);
		FIXED cosine = lu_cos(luAngle);

		FIXED tan = fxdiv(sine, cosine);
		//avoid division by zero later on
		if (rayAngle == 0) {
			tan = float2fx(0.01);
		}




		float cosineFloat = fx2float(lu_cos(luAngle));
		FIXED cosineFixed = (lu_cos(luAngle));

		FIXED horizontalDistance = int2fx(-1);

		FIXED verticalDistance = int2fx(-1);


		/*
		this part checks for horizontal walls on the 2d map
		*/
		
		//initial coordinate to check for horizontal walls
		FIXED tx;
		FIXED ty;
		//lengths of further steps that will be taken for checking collisions
		FIXED dx;
		FIXED dy;
		if (rayAngle >= 0 && rayAngle < 180) {
			ty = int2fx(y / TILESIZE * TILESIZE -1);
			dy = fxsub(int2fx(0), FIXEDTILESIZE);
		}
		else {
			ty = int2fx(y / TILESIZE * TILESIZE + TILESIZE);
			dy = FIXEDTILESIZE;
		}
		//tx = x + fx2float(fxmul(fxdiv(float2fx(floatAbs(y- fx2float(ty))), tan), int2fx(xDir)));
		//tx = x + fx2float(fxmul(fxdiv(fixedAbs(fxsub(fixedY,  ty)), tan), int2fx(xDir)));
		tx = (fxadd(fixedX, (fxmul(fxdiv(fixedAbs(fxsub(fixedY,  ty)), tan), int2fx(xDir)))));
   		dx = fxmul((fxdiv(FIXEDTILESIZE, tan)), int2fx(xDir));

		

		//FIXED tx2 = float2fx(tx);

		int initX = fx2int(tx) / TILESIZE;
		int initY = fx2int(ty) / TILESIZE;




		if (MAP[initX][initY] != 0) {
			//horizontalDistance = float2fx(floatAbs((x - fx2float(tx)) / cosineFloat));
			horizontalDistance = float2fx(floatAbs(fx2float(fxsub(fixedX, tx)) / cosineFloat));
		}
		
		else {
			//step tx & ty up in steps of dx & dy 
			for (int j = 0; j < 20; j++) {
				//tx += dx;
				//ty += dy;
				tx = fxadd((tx), dx);
				ty = fxadd(ty, dy);
				
				initX = (fx2int(tx)) / TILESIZE;
				initY = (fx2int(ty)) / TILESIZE;
				if (MAP[initX][initY] != 0) {
					//horizontalDistance = float2fx(floatAbs((x - fx2float(tx)) / cosineFloat));
					horizontalDistance = float2fx(floatAbs(fx2float(fxsub(fixedX, tx)) / cosineFloat));
					break;
				}
			}

		}
		
		

		
		
		



		/*
		this part checks for vertical walls on the 2d map
		*/
		
		int scaledTan = fx2int(fxmul(tan, 1024));
		//initial coordinate to check for horizontal walls
		float txh, tyh;
		//lengths of further steps that will be taken for checking collisions
		float dxh, dyh;

		if (rayAngle >= 90 && rayAngle < 270) {
			txh = x / TILESIZE * TILESIZE -1;
			dxh = -TILESIZE;
		}
		else {
			txh = x / TILESIZE * TILESIZE + TILESIZE;
			dxh = TILESIZE;
		}
		tyh = y + fx2float(fxmul(float2fx(floatAbs(x-txh)), tan))*yDir; //64 to correct for tan scaling
   		dyh = fx2float(fxmul(FIXEDTILESIZE, tan))*yDir; //64 to correct for tan scaling


		int initXH = ((int)txh) / TILESIZE;
		int initYH = ((int)tyh) / TILESIZE;


		//return tan*100;
		//return MAP[initXH][initYH];


		
			if (MAP[initXH][initYH] != 0) {

				verticalDistance = float2fx(floatAbs((x - txh) / cosineFloat));

			}
			else {
				//step tx & ty up in steps of dx & dy 
				for (int j = 0; j < 20; j++) {

					txh += dxh;
					tyh += dyh;

					initXH = ((int)txh) / TILESIZE;
					initYH = ((int)tyh) / TILESIZE;

					if (MAP[initXH][initYH] != 0) {
						verticalDistance = float2fx(floatAbs((x - txh) / cosineFloat));
						break;
					}
				}

			}

		
		
		
		

		//return verticalDistance;
		//return initX;
		//return MAP[0][0];


		if (verticalDistance >= 0 && (verticalDistance <= horizontalDistance || horizontalDistance < 0)) {
			drawWall(i, verticalDistance, MAP[initXH][initYH]);
		}
		
		else if (horizontalDistance >= 0 && (horizontalDistance < verticalDistance || verticalDistance < 0)) {
			drawWall(i, horizontalDistance, MAP[initX][initY]);
		}
		else {
			m3_line(i, 0, i, 160, RGB15(0,0,0));
		}






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

	int x = 2*64;//96;//2*64;//
	int y = 2*64;//224;//2*64;//

	int direction = 230; //315, 0,0

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
		direction += 3;
		//x = fxadd(x, float2fx(0.1));
		//y = fxadd(y, float2fx(0.1));
		if (direction >= 360) {
			direction = 0;
		}
		
		
		
		
		//VBlankIntrWait();
	}
}
