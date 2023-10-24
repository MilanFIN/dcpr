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
};

struct Player {
	bool hasKey;
	int hp;
};
