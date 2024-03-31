#ifndef STRUCTS_H
#define STRUCTS_H
#include <tonc.h>

/// @brief an actor in the game that is drawn with a sprite
struct Entity
{
	int active;
	int moving;
	FIXED x;
	FIXED y;
	FIXED xDir;
	FIXED yDir;
	int type; // 1: key, 2: projectile, 3:enemy, 4 hp pack, 5 gun level up, 6 mana, 7 splatter
	int texture;
	FIXED distance;
	FIXED scale;
	int yOffset;
	FIXED speed;
	int actionDelay;
	int actionFrequency;
	// amount of damage dealth or healed
	int damage;
	int level;
	int hp;
	int hit; // set to 1, when should show hit frame
	int notification[10];
	int notificationLength;
	int mask;
};

struct Player
{
	FIXED x;
	FIXED y;
	FIXED direction;
	int hasKey;
	int hp;
	int maxHp;
	int ammo;
	int maxAmmo;
	int ammoRecoveryRate;
	int gunLevel;
	int maxGunLevel;
	int damage;
	int overdrive; // not using ammo when this is != 0, counts toward 0
	int maxOverDrive;
	FIXED speed;
	// in map tile space
	int previousX;
	int previousY;
};

extern struct Player player;

#endif // STRUCTS_H