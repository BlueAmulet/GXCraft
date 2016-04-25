#include <grrlib.h>

#include "../main.h"
#include "../block.h"
#include "../render.h"
#include "../display_list.h"

#include "water.h"

blockTexture *tex_water;

inline void drawWaterBlock(int xPos, int yPos, int zPos);

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawWaterBlock(xPos, yPos, zPos);
}

void water_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(8, entry);
	tex_water = getTexture(14, 0);
}

bool testWaterFace(unsigned char face) {
	return face == 0 || face == 6 || face == 18 || face == 20 || face == 37 || face == 38 || face == 39 || face == 40;
}

inline void drawWaterBlock(int xPos, int yPos, int zPos) {
	bool drawBack  = zPos >= worldZ-1 || testWaterFace(theWorld[yPos][xPos][zPos + 1]);
	bool drawFront = zPos <= 0        || testWaterFace(theWorld[yPos][xPos][zPos - 1]);
	bool drawRight = xPos >= worldX-1 || testWaterFace(theWorld[yPos][xPos + 1][zPos]);
	bool drawLeft  = xPos <= 0        || testWaterFace(theWorld[yPos][xPos - 1][zPos]);
	bool drawTop   = yPos >= worldY-1 || testWaterFace(theWorld[yPos + 1][xPos][zPos]);
	bool drawBott  = yPos > 0         && testWaterFace(theWorld[yPos - 1][xPos][zPos]);

	int size = 0;
	if (drawBack)  size += 4;
	if (drawFront) size += 4;
	if (drawRight) size += 4;
	if (drawLeft)  size += 4;
	if (drawTop)   size += 4;
	if (drawBott)  size += 4;

	if (size == 0) return;

	f32 u0 = tex_water->u0, v0 = tex_water->v0;
	f32 u1 = tex_water->u1, v1 = tex_water->v1;

	if (drawBack) {
		displist_add(xPos, 1+yPos, 1+zPos,
		0xCCCF,
		u0,v0);
		displist_add( 1+xPos, 1+yPos, 1+zPos,
		0xCCCF,
		u1,v0);
		displist_add( 1+xPos,yPos, 1+zPos,
		0xCCCF,
		u1,v1);
		displist_add(xPos,yPos, 1+zPos,
		0xCCCF,
		u0,v1);
	}

	if (drawFront) {
		displist_add( 1+xPos, 1+yPos,zPos,
		0xCCCF,
		u0,v0);
		displist_add(xPos, 1+yPos,zPos,
		0xCCCF,
		u1,v0);
		displist_add(xPos,yPos,zPos,
		0xCCCF,
		u1,v1);
		displist_add( 1+xPos,yPos,zPos,
		0xCCCF,
		u0,v1);
	}

	if (drawRight) {
		displist_add( 1+xPos, 1+yPos, 1+zPos,
		0x999F,
		u0,v0);
		displist_add( 1+xPos, 1+yPos,zPos,
		0x999F,
		u1,v0);
		displist_add( 1+xPos,yPos,zPos,
		0x999F,
		u1,v1);
		displist_add( 1+xPos,yPos, 1+zPos,
		0x999F,
		u0,v1);
	}

	if (drawLeft) {
		displist_add(xPos, 1+yPos,zPos,
		0x999F,
		u0,v0);
		displist_add(xPos, 1+yPos, 1+zPos,
		0x999F,
		u1,v0);
		displist_add(xPos,yPos, 1+zPos,
		0x999F,
		u1,v1);
		displist_add(xPos,yPos,zPos,
		0x999F,
		u0,v1);
	}

	if (drawTop) {
		unsigned short c = lighting[xPos][zPos] <= yPos ? 0xFFFF : 0x999F;
		displist_add(xPos, 1+yPos,zPos,
		c,
		u0,v0);
		displist_add( 1+xPos, 1+yPos,zPos,
		c,
		u1,v0);
		displist_add( 1+xPos, 1+yPos, 1+zPos,
		c,
		u1,v1);
		displist_add(xPos, 1+yPos, 1+zPos,
		c,
		u0,v1);
	}

	if (drawBott) {
		displist_add( 1+xPos,yPos,zPos,
		0x555F,
		u0,v0);
		displist_add(xPos,yPos,zPos,
		0x555F,
		u1,v0);
		displist_add(xPos,yPos, 1+zPos,
		0x555F,
		u1,v1);
		displist_add( 1+xPos,yPos, 1+zPos,
		0x555F,
		u0,v1);
	}
}
