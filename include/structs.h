struct Entity {
	bool active;
	bool moving;
	FIXED x;
	FIXED y;
	int type;
	FIXED distance;
	FIXED scale;
};

struct Player {
	bool hasKey;
	int hp;
};