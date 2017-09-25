#pragma once
#include <cstdint>
#include <grrlib.h>

struct HitBox {
	double width;
	double height;
};

class Entity : public HitBox {
	public:
		f32 posX;
		f32 posY;
		f32 posZ;
		double motionX;
		double motionY;
		double motionZ;
		f32 lookX;
		f32 lookY;
		f32 lookZ;

		void moveEntity(double);
};

class Player : public Entity {
	public:
		uint8_t inventory[10];
		bool flying;
		bool select;
		double timer;

		Player();
};
