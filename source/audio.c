#include "audio.h"

void initAudio()
{

	REG_SNDSTAT = SSTAT_ENABLE;
	REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1 | SDMG_SQR2 | SDMG_NOISE, 7);

	REG_SNDDSCNT = SDS_DMG100;
	REG_SND1CNT = SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
	REG_SND1SWEEP = SSW_INC | SSW_TIME(5) | SSW_SHIFT(5);

	REG_SND1FREQ = 0;
	REG_SND2FREQ = 0;
	REG_SND4FREQ = 0;

}

// ei toimi:
// key, menu ok

void playSound(int id)
{
	// blip
	if (id == 0)
	{
		const uint8_t volume = 9;	 // 0-15
		const uint8_t direction = 0; // 0-1 envelope dir, 0 for down
		const uint8_t step = 1;		 // 0-7 envelope step time
		const uint16_t duty = 2;	 // duty cycle, 0-3
		const uint8_t length = 63;	 // length 0-63

		REG_SND1CNT = volume << 12 | direction << 11 | step << 8 | duty << 6 | length;
		REG_SND1SWEEP = SSW_DEC | SSW_TIME(1) | SSW_SHIFT(5);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_D, 0) | SFREQ_HOLD; // SFREQ_TIMED;
	}
	// basic player getting hit sound
	else if (id == 1)
	{
		REG_SND4CNT = SSQR_ENV_BUILD(12, 0, 2); // | SSQR_DUTY1_4;
		REG_SND4FREQ = SFREQ_RESET | SND_RATE(NOTE_F, 0);
	}
	// hp pickup
	else if (id == 2)
	{
		REG_SND1CNT = SSQR_ENV_BUILD(10, 0, 2) | SSQR_DUTY1_2;
		REG_SND1SWEEP = SSW_INC | SSW_TIME(7) | SSW_SHIFT(4);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_G, -1) | SFREQ_HOLD; // SFREQ_TIMED;
	}
	// spell pickup
	else if (id == 3)
	{
		const uint8_t volume = 15;	 // 0-15
		const uint8_t direction = 0; // 0-1 envelope dir, 0 for down
		const uint8_t step = 1;		 // 0-7 envelope step time
		const uint16_t duty = 2;	 // duty cycle, 0-3
		const uint8_t length = 7;	 // length 0-63

		REG_SND1CNT = volume << 12 | direction << 11 | step << 8 | duty << 6 | length;
		REG_SND1SWEEP = SSW_INC | SSW_TIME(7) | SSW_SHIFT(3);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_C, -1) | SFREQ_HOLD; // SFREQ_TIMED;
	}
	// noise
	else if (id == 4)
	{
		REG_SND4CNT = SSQR_ENV_BUILD(12, 0, 2) | SSQR_DUTY1_4;
		REG_SND4FREQ = SFREQ_RESET | SND_RATE(NOTE_A, 1) | SFREQ_TIMED;
	}
	// menu cancel
	else if (id == 5)
	{
		REG_SND4CNT = SSQR_ENV_BUILD(12, 0, 2) | SSQR_DUTY1_2; // 4;
		REG_SND4FREQ = SFREQ_RESET | SND_RATE(NOTE_F, -4) | SFREQ_TIMED;
	}
	else if (id == 6)
	{
		REG_SND4CNT = SSQR_ENV_BUILD(12, 0, 0) | SSQR_DUTY1_8;
		REG_SND4FREQ = SFREQ_RESET | SND_RATE(NOTE_G, -2) | SFREQ_TIMED;
	}
	// start
	else if (id == 7)
	{
		REG_SND2CNT = SSQR_ENV_BUILD(12, 0, 4) | SSQR_DUTY1_4;
		REG_SND2FREQ = SFREQ_RESET | SND_RATE(NOTE_G, -2);
	}
	// maybe door open?
	else if (id == 8)
	{

		REG_SND1CNT = SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
		REG_SND1SWEEP = SSW_INC | SSW_TIME(7) | SSW_SHIFT(3);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_D, -2);
	}
	// menu move
	else if (id == 9)
	{

		REG_SND1CNT = SSQR_ENV_BUILD(8, 0, 7) | SSQR_DUTY1_2;
		REG_SND1SWEEP = SSW_INC | SSW_TIME(5) | SSW_SHIFT(5);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_C, 2);
	}
	// menu ok
	else if (id == 10)
	{

		REG_SND1CNT = SSQR_ENV_BUILD(10, 0, 7) | SSQR_DUTY1_2;
		REG_SND1SWEEP = SSW_INC | SSW_TIME(5) | SSW_SHIFT(5);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_C, -1);
	}
	// death sound (noise)?
	else if (id == 11)
	{
		REG_SND4CNT = SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_8;
		REG_SND4FREQ = SFREQ_RESET | SND_RATE(NOTE_E, 0);
	}
	// enemy death sound
	else if (id == 12)
	{
		REG_SND1CNT = SSQR_ENV_BUILD(6, 0, 3) | SSQR_DUTY1_8;
		REG_SND1SWEEP = SSW_DEC | SSW_TIME(1) | SSW_SHIFT(7);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_C, -1);
	}
	// fire 1
	else if (id == 13)
	{
		REG_SND2CNT = SSQR_ENV_BUILD(7, 0, 2) | SSQR_DUTY1_2;
		REG_SND2FREQ = SFREQ_RESET | SND_RATE(NOTE_CIS, -1) | SFREQ_TIMED;
	}
	// fire 2
	else if (id == 14)
	{
		REG_SND2CNT = SSQR_ENV_BUILD(7, 0, 5) | SSQR_DUTY1_2;
		REG_SND2FREQ = SFREQ_RESET | SND_RATE(NOTE_C, -2) | SFREQ_TIMED;
	}
	// fire sound 3
	else if (id == 15)
	{
		REG_SND2CNT = SSQR_ENV_BUILD(7, 0, 4) | SSQR_DUTY1_4;
		REG_SND2FREQ = SFREQ_RESET | SND_RATE(NOTE_CIS, -2) | SFREQ_TIMED;
	}
	// key
	else if (id == 16)
	{
		//return 0;

		REG_SND1CNT = SSQR_ENV_BUILD(7, 0, 7) | SSQR_DUTY1_2;
		REG_SND1SWEEP = SSW_INC | SSW_TIME(7) | SSW_SHIFT(7);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_A, 1) | SFREQ_TIMED;
	}
	// door locked
	else if (id == 17)
	{
		REG_SND4CNT = SSQR_ENV_BUILD(7, 0, 5) | SSQR_DUTY1_8;
		REG_SND4FREQ = SFREQ_RESET | SND_RATE(NOTE_FIS, -2) | SFREQ_TIMED;
	}
	// door open
	else if (id == 18)
	{
		//return 0;

		REG_SND1CNT = SSQR_ENV_BUILD(10, 0, 7) | SSQR_DUTY1_2;
		REG_SND1SWEEP = SSW_INC | SSW_TIME(3) | SSW_SHIFT(6);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_A, -1) | SFREQ_HOLD;
	}
	// mana pickup
	else if (id == 19)
	{
		//return 0;

		REG_SND1CNT = SSQR_ENV_BUILD(12, 0, 2) | SSQR_DUTY1_2;
		REG_SND1SWEEP = SSW_INC | SSW_TIME(7) | SSW_SHIFT(3);
		REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_DIS, -1) | SFREQ_HOLD;
	}
}
