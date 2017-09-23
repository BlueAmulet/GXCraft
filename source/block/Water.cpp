#include <grrlib.h>

#include "../Main.hpp"
#include "../Block.hpp"
#include "../Render.hpp"
#include "../DisplayList.hpp"

#include "Water.hpp"

static blockTexture *tex_water;

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
	bool drawBack  = zPos >= worldZ-1 || testWaterFace(theWorld->theWorld[yPos][xPos][zPos + 1]);
	bool drawFront = zPos <= 0        || testWaterFace(theWorld->theWorld[yPos][xPos][zPos - 1]);
	bool drawRight = xPos >= worldX-1 || testWaterFace(theWorld->theWorld[yPos][xPos + 1][zPos]);
	bool drawLeft  = xPos <= 0        || testWaterFace(theWorld->theWorld[yPos][xPos - 1][zPos]);
	bool drawTop   = yPos >= worldY-1 || testWaterFace(theWorld->theWorld[yPos + 1][xPos][zPos]);
	bool drawBott  = yPos > 0         && testWaterFace(theWorld->theWorld[yPos - 1][xPos][zPos]);

	if (!drawBack && !drawFront && !drawRight && !drawLeft && !drawTop && !drawBott) return;

	DisplayList *dlist = Render::getbound();

	f32 u0 = tex_water->u0, v0 = tex_water->v0;
	f32 u1 = tex_water->u1, v1 = tex_water->v1;

	if (drawBack) {
		unsigned short c = (zPos < worldZ-1 && theWorld->lighting[xPos][zPos+1] > yPos) ? 0x888F : 0xCCCF;

		dlist->add(xPos, 1+yPos, 1+zPos,
		c,
		u0,v0);
		dlist->add( 1+xPos, 1+yPos, 1+zPos,
		c,
		u1,v0);
		dlist->add( 1+xPos,yPos, 1+zPos,
		c,
		u1,v1);
		dlist->add(xPos,yPos, 1+zPos,
		c,
		u0,v1);
	}

	if (drawFront) {
		unsigned short c = (zPos > 0 && theWorld->lighting[xPos][zPos-1] > yPos) ? 0x888F : 0xCCCF;

		dlist->add( 1+xPos, 1+yPos,zPos,
		c,
		u0,v0);
		dlist->add(xPos, 1+yPos,zPos,
		c,
		u1,v0);
		dlist->add(xPos,yPos,zPos,
		c,
		u1,v1);
		dlist->add( 1+xPos,yPos,zPos,
		c,
		u0,v1);
	}

	if (drawRight) {
		unsigned short c = (xPos < worldX-1 && theWorld->lighting[xPos+1][zPos] > yPos) ? 0x777F : 0x999F;

		dlist->add( 1+xPos, 1+yPos, 1+zPos,
		c,
		u0,v0);
		dlist->add( 1+xPos, 1+yPos,zPos,
		c,
		u1,v0);
		dlist->add( 1+xPos,yPos,zPos,
		c,
		u1,v1);
		dlist->add( 1+xPos,yPos, 1+zPos,
		c,
		u0,v1);
	}

	if (drawLeft) {
		unsigned short c = (xPos > 0 && theWorld->lighting[xPos-1][zPos] > yPos) ? 0x777F : 0x999F;

		dlist->add(xPos, 1+yPos,zPos,
		c,
		u0,v0);
		dlist->add(xPos, 1+yPos, 1+zPos,
		c,
		u1,v0);
		dlist->add(xPos,yPos, 1+zPos,
		c,
		u1,v1);
		dlist->add(xPos,yPos,zPos,
		c,
		u0,v1);
	}

	if (drawTop) {
		unsigned short c = (theWorld->lighting[xPos][zPos] > yPos) ? 0x999F : 0xFFFF;

		dlist->add(xPos, 1+yPos,zPos,
		c,
		u0,v0);
		dlist->add( 1+xPos, 1+yPos,zPos,
		c,
		u1,v0);
		dlist->add( 1+xPos, 1+yPos, 1+zPos,
		c,
		u1,v1);
		dlist->add(xPos, 1+yPos, 1+zPos,
		c,
		u0,v1);
	}

	if (drawBott) {
		dlist->add( 1+xPos,yPos,zPos,
		0x555F,
		u0,v0);
		dlist->add(xPos,yPos,zPos,
		0x555F,
		u1,v0);
		dlist->add(xPos,yPos, 1+zPos,
		0x555F,
		u1,v1);
		dlist->add( 1+xPos,yPos, 1+zPos,
		0x555F,
		u0,v1);
	}
}
