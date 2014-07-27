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
	bool flying;
	unsigned short timer;
} player;
