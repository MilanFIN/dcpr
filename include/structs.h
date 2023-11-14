struct Entity {
	bool active;
	bool moving;
	FIXED x;
	FIXED y;
	FIXED xDir;
	FIXED yDir;
	int type; //1: key, 2: projectile, 3:enemy
	int texture;
	FIXED distance;
	FIXED scale;
	FIXED yOffset;
	FIXED speed;
	int attackDelay;
	int attackFrequency;
	//amount of damage dealth or healed
	int damage;
	int level;
	int hp;
};

struct Player {
	bool hasKey;
	int hp;
	int maxHp;
	int gunLevel;
	int maxGunLevel;
	int damage;
};
