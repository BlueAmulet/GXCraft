#pragma once
typedef struct player_s {
	f32 posX;
	f32 posY;
	f32 posZ;
	double motionX;
	double motionY;
	double motionZ;
	f32 lookX;
	f32 lookY;
	f32 lookZ;
	u8 inventory[10];
	bool flying;
	bool select;
	double timer;
} player;
