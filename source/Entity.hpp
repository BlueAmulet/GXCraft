#pragma once

#include <cstdint>
#include <gctypes.h>

struct HitBox {
	double width;
	double height;
};

class Entity : public HitBox {
public:
	f32 posX;
	f32 posY;
	f32 posZ;
	f32 lookX;
	f32 lookY;
	f32 lookZ;
	double motionX;
	double motionY;
	double motionZ;

	bool isStuck();
	void moveEntity(double);
};

class Player : public Entity {
public:
	double timer;
	uint8_t inventory[10];
	bool flying;
	bool select;

	Player();
};
