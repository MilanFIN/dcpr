#include "timer.h"

void startTimer1s()
{
	// reset timers
	REG_TM2D = 0;
	REG_TM3D = 0;
	REG_TM2CNT = 0;
	REG_TM3CNT = 0;
	// init
	REG_TM2D = -0x4000;		   // 0x4000 ticks till overflow
	REG_TM2CNT = TM_FREQ_1024; // we're using the 1024 cycle timer

	// 0x4000 * 1024 cycles (61us) ~1s

	// cascade into tm3
	REG_TM3CNT = TM_ENABLE | TM_CASCADE;
	// start timer
	REG_TM2CNT |= TM_ENABLE;
}

void pauseTimer()
{
	REG_TM2CNT ^= TM_CASCADE;
}

void resumeTimer()
{
	REG_TM2CNT ^= TM_CASCADE;
}

int readTimer()
{
	if (REG_TM3D != -1)
	{
		return REG_TM3D;
	}
	else
	{
		return 0;
	}
}