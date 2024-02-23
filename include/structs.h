#ifndef STRUCTS_H
#define STRUCTS_H
#include <tonc.h>


/// @brief an actor in the game that is drawn with a sprite
struct Entity
{
	bool active;
	bool moving;
	FIXED x;
	FIXED y;
	FIXED xDir;
	FIXED yDir;
	char type; // 1: key, 2: projectile, 3:enemy, 4 hp pack, 5 gun level up
	char texture;
	FIXED distance;
	FIXED scale;
	FIXED yOffset;
	FIXED speed;
	char attackDelay;
	char attackFrequency;
	// amount of damage dealth or healed
	int damage;
	char level;
	int hp;
	char hit; // set to 1, when should show hit frame
	char notification[4];
	size_t notificationLength;
};


struct Player
{
	FIXED x;
	FIXED y;
	FIXED direction;
	bool hasKey;
	int hp;
	int maxHp;
	int ammo;
	int maxAmmo;
	char gunLevel;
	char maxGunLevel;
	char damage;
	FIXED speed;
	//in map tile space
	int previousX;
	int previousY;
};

extern struct Player player;


#endif // STRUCTS_H