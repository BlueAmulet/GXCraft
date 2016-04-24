#pragma once
typedef struct player_s {
	double posX;
	double posY;
	double posZ;
	double motionX;
	double motionY;
	double motionZ;
	double lookX;
	double lookY;
	double lookZ;
	u8 inventory[10];
	bool flying;
	bool select;
	unsigned short timer;
} player;
