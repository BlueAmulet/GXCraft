#include <cmath>

#include "Entity.hpp"
#include "Main.hpp"

#define blockclampW(a, b) (a)=floor(a) + (((b) < 0.0) ? (0.5001 - (1.0 - width)/2.0) : (0.4999 + (1.0 - width)/2.0))
#define blockclampH(a, b) (a)=floor(a) + (((b) < 0.0) ? 0.0 : (ceil(height) - 0.0001 - (height)))

#define getBlockAround(xOff, yOff, zOff) theWorld->getBlock(floor(this->posX+xOff), floor(this->posY+yOff), floor(this->posZ+zOff))
#define testBlock(block) (block == 0 || block == 8 || block == 10)

// Not so super simple collision checking, may eventually look into AABB
bool Entity::isStuck() {
	double width = this->width;
	double height = this->height;

	uint8_t block1, block2, block3, block4, block5, block6, block7, block8;
	block1 = getBlockAround(-width/2.0,    0.0, -width/2.0);
	block2 = getBlockAround(-width/2.0,    0.0,  width/2.0);
	block3 = getBlockAround( width/2.0,    0.0, -width/2.0);
	block4 = getBlockAround( width/2.0,    0.0,  width/2.0);
	block5 = getBlockAround(-width/2.0, height, -width/2.0);
	block6 = getBlockAround(-width/2.0, height,  width/2.0);
	block7 = getBlockAround( width/2.0, height, -width/2.0);
	block8 = getBlockAround( width/2.0, height,  width/2.0);
	return (!testBlock(block1) || !testBlock(block2) || !testBlock(block3) || !testBlock(block4) || !testBlock(block5) || !testBlock(block6) || !testBlock(block7) || !testBlock(block8));
}

void Entity::moveEntity(double deltaTime) {
	double motionVX = this->motionX * deltaTime;
	double motionVY = this->motionY * deltaTime;
	double motionVZ = this->motionZ * deltaTime;

	if (motionVX == 0.0 && motionVY == 0.0 && motionVZ == 0.0)
		return;

	if (this->isStuck()) {
		// Allow player to get out of ground if they glitch inside
		this->posX += motionVX;
		this->posY += motionVY;
		this->posZ += motionVZ;
		return;
	}

	double width = this->width;
	double height = this->height;

	uint8_t block1, block2, block3, block4, block5, block6, block7, block8;

// This looks like garbage, and it is, but with compiler optimizations isn't so bad.
#define testBlockPos(vx, vy, vz) \
	block1 = getBlockAround(-width/2.0+vx,    0.0+vy, -width/2.0+vz);\
	block2 = getBlockAround(-width/2.0+vx,    0.0+vy,  width/2.0+vz);\
	block3 = getBlockAround( width/2.0+vx,    0.0+vy, -width/2.0+vz);\
	block4 = getBlockAround( width/2.0+vx,    0.0+vy,  width/2.0+vz);\
	block5 = getBlockAround(-width/2.0+vx, height+vy, -width/2.0+vz);\
	block6 = getBlockAround(-width/2.0+vx, height+vy,  width/2.0+vz);\
	block7 = getBlockAround( width/2.0+vx, height+vy, -width/2.0+vz);\
	block8 = getBlockAround( width/2.0+vx, height+vy,  width/2.0+vz);\
	if (testBlock(block1) && testBlock(block2) && testBlock(block3) && testBlock(block4) && testBlock(block5) && testBlock(block6) && testBlock(block7) && testBlock(block8)) {\
		if (!__builtin_strcmp(#vx, "0.0"))\
			blockclampW(this->posX, motionVX);\
		else\
			this->posX += motionVX;\
		if (!__builtin_strcmp(#vy, "0.0"))\
			blockclampH(this->posY, motionVY);\
		else\
			this->posY += motionVY;\
		if (!__builtin_strcmp(#vz, "0.0"))\
			blockclampW(this->posZ, motionVZ);\
		else\
			this->posZ += motionVZ;\
		return;\
	}
	testBlockPos(motionVX, motionVY, motionVZ);
	testBlockPos(motionVX,      0.0, motionVZ);
	testBlockPos(     0.0, motionVY, motionVZ);
	testBlockPos(motionVX, motionVY,      0.0);
	testBlockPos(     0.0,      0.0, motionVZ);
	testBlockPos(motionVX,      0.0,      0.0);
	testBlockPos(     0.0, motionVY,      0.0);
}
#undef testBlockPos
#undef blockclampW
#undef blockclampH
#undef getBlockAround
#undef testBlock

Player::Player() {
	this->width = 0.6;
	this->height = 1.8;
}
