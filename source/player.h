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
	unsigned char inventory[10];
	bool flying;
	unsigned short timer;
} player;
