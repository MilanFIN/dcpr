#ifndef AUDIO_H
#define AUDIO_H

#include <tonc.h>

/// @brief sets gba audio registers to play dmg sounds
void initAudio();
/// @brief main sound triggering function
/// @param id selector between 0...n of the defined sounds
void playSound(int id);

#endif