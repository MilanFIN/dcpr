#ifndef TIMER_H
#define TIMER_H

#include <tonc.h>

void startTimer1s();

void startSeedTimer();

void pauseTimer();

void resumeTimer();

int readSeedTimer();

int readTimer();
#endif // TIMER_H