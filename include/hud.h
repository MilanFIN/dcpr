#ifndef HUD_H
#define HUD_H

#include <tonc.h>
#include "utils.h"
#include "structs.h"
#include "render.h"
#include "textures.h"

extern const int notificationDuration;
extern const int HUDHEIGHT;

extern int updateHud;
extern int updateCompass;
extern char notification[5];
extern size_t notificationLength;
extern int notificationCounter;
extern int compassDirection;

void setNotification(struct Entity *entity);

void drawHud();

void drawCompass();

void updateAmmo();

void checkCompass();

#endif // HUD_H