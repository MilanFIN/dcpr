#ifndef TOPLAYER_H
#define TOPLAYER_H

#include <tonc.h>
#include "textures.h"
#include "render.h"
#include "hud.h"

#define OFFSETFRAMES 14

extern int offsetFrame;
extern const int HANDXOFFSETS[OFFSETFRAMES];
extern const int HANDYOFFSETS[OFFSETFRAMES];

extern bool movedThisFrame;

void moveHands();

void drawHands();

#endif // TOPLAYER_H