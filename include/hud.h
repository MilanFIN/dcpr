#ifndef HUD_H
#define HUD_H

#include <tonc.h>
#include "utils.h"
#include "structs.h"
#include "render.h"
#include "textures.h"

extern int updateHud;
extern char notification[5];
extern size_t notificationLength;
extern int notificationCounter;
extern const int notificationDuration;
extern const int HUDHEIGHT;

void setNotification(struct Entity *entity);

void drawHud();

void updateAmmo();

void drawCompass();

#endif // HUD_H