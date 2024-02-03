#ifndef STRUCTS_H
#define STRUCTS_H

struct Entity
{
	bool active;
	bool moving;
	FIXED x;
	FIXED y;
	FIXED xDir;
	FIXED yDir;
	char type; // 1: key, 2: projectile, 3:enemy
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
	char gunLevel;
	char maxGunLevel;
	char damage;
	FIXED speed;
};

#endif // STRUCTS_H