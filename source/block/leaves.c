#include <grrlib.h>

#include "../main.h"
#include "../block.h"
#include "../render.h"
#include "../textures/block_leaves.h"
#include "leaves.h"

GRRLIB_texImg *tex_leaves;

inline void drawLeavesBlock(int xPos, int yPos, int zPos, GRRLIB_texImg *tex);

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawLeavesBlock(xPos, yPos, zPos, tex_leaves);
}

void leaves_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(18, entry);
	tex_leaves = GRRLIB_LoadTexture(block_leaves);
}

void leaves_clean() {
	GRRLIB_FreeTexture(tex_leaves);
}

bool testLeavesFace(unsigned char face) {
	return face == 0 || face == 6 || face == 8 || face == 37 || face == 38 || face == 39 || face == 40;
}

inline void drawLeavesBlock(int xPos, int yPos, int zPos, GRRLIB_texImg *tex) {

	/*bool drawBack  = zPos >= worldZ-1 || testLeavesFace(theWorld[yPos][xPos][zPos + 1]);
	bool drawFront = zPos <= 0        || testLeavesFace(theWorld[yPos][xPos][zPos - 1]);
	bool drawRight = xPos >= worldX-1 || testLeavesFace(theWorld[yPos][xPos + 1][zPos]);
	bool drawLeft  = xPos <= 0        || testLeavesFace(theWorld[yPos][xPos - 1][zPos]);
	bool drawTop   = yPos >= worldY-1 || testLeavesFace(theWorld[yPos + 1][xPos][zPos]);
	bool drawBott  = yPos > 0         && testLeavesFace(theWorld[yPos - 1][xPos][zPos]);

	int size = 0;
	if (drawBack)  size += 4;
	if (drawFront) size += 4;
	if (drawRight) size += 4;
	if (drawLeft)  size += 4;
	if (drawTop)   size += 4;
	if (drawBott)  size += 4;
	
	if (size == 0) return;
	
	if (lastTex != tex)
		GRRLIB_SetTexture(tex, 0);
	lastTex = tex;

	GX_Begin(GX_QUADS, GX_VTXFMT1, size);

	if (drawBack) {
		GX_Position3s16(xPos, 1+yPos, 1+zPos);
		GX_Color1u16(0xCCCF);
		GX_TexCoord2u8(0,0);
		GX_Position3s16( 1+xPos, 1+yPos, 1+zPos);
		GX_Color1u16(0xCCCF);
		GX_TexCoord2u8(1,0);
		GX_Position3s16( 1+xPos,yPos, 1+zPos);
		GX_Color1u16(0xCCCF);
		GX_TexCoord2u8(1,1);
		GX_Position3s16(xPos,yPos, 1+zPos);
		GX_Color1u16(0xCCCF);
		GX_TexCoord2u8(0,1);
	}

	if (drawFront) {
		GX_Position3s16( 1+xPos, 1+yPos,zPos);
		GX_Color1u16(0xCCCF);
		GX_TexCoord2u8(0,0);
		GX_Position3s16(xPos, 1+yPos,zPos);
		GX_Color1u16(0xCCCF);
		GX_TexCoord2u8(1,0);
		GX_Position3s16(xPos,yPos,zPos);
		GX_Color1u16(0xCCCF);
		GX_TexCoord2u8(1,1);
		GX_Position3s16( 1+xPos,yPos,zPos);
		GX_Color1u16(0xCCCF);
		GX_TexCoord2u8(0,1);
	}

	if (drawRight) {
		GX_Position3s16( 1+xPos, 1+yPos, 1+zPos);
		GX_Color1u16(0x999F);
		GX_TexCoord2u8(0,0);
		GX_Position3s16( 1+xPos, 1+yPos,zPos);
		GX_Color1u16(0x999F);
		GX_TexCoord2u8(1,0);
		GX_Position3s16( 1+xPos,yPos,zPos);
		GX_Color1u16(0x999F);
		GX_TexCoord2u8(1,1);
		GX_Position3s16( 1+xPos,yPos, 1+zPos);
		GX_Color1u16(0x999F);
		GX_TexCoord2u8(0,1);
	}

	if (drawLeft) {
		GX_Position3s16(xPos, 1+yPos,zPos);
		GX_Color1u16(0x999F);
		GX_TexCoord2u8(0,0);
		GX_Position3s16(xPos, 1+yPos, 1+zPos);
		GX_Color1u16(0x999F);
		GX_TexCoord2u8(1,0);
		GX_Position3s16(xPos,yPos, 1+zPos);
		GX_Color1u16(0x999F);
		GX_TexCoord2u8(1,1);
		GX_Position3s16(xPos,yPos,zPos);
		GX_Color1u16(0x999F);
		GX_TexCoord2u8(0,1);
	}

	if (drawTop) {
		unsigned short c = lighting[xPos][zPos] <= yPos ? 0xFFFF : 0x999F;

		GX_Position3s16(xPos, 1+yPos,zPos);
		GX_Color1u16(c);
		GX_TexCoord2u8(0,0);
		GX_Position3s16( 1+xPos, 1+yPos,zPos);
		GX_Color1u16(c);
		GX_TexCoord2u8(1,0);
		GX_Position3s16( 1+xPos, 1+yPos, 1+zPos);
		GX_Color1u16(c);
		GX_TexCoord2u8(1,1);
		GX_Position3s16(xPos, 1+yPos, 1+zPos);
		GX_Color1u16(c);
		GX_TexCoord2u8(0,1);
	}

	if (drawBott) {
		GX_Position3s16( 1+xPos,yPos,zPos);
		GX_Color1u16(0x555F);
		GX_TexCoord2u8(0,0);
		GX_Position3s16(xPos,yPos,zPos);
		GX_Color1u16(0x555F);
		GX_TexCoord2u8(1,0);
		GX_Position3s16(xPos,yPos, 1+zPos);
		GX_Color1u16(0x555F);
		GX_TexCoord2u8(1,1);
		GX_Position3s16( 1+xPos,yPos, 1+zPos);
		GX_Color1u16(0x555F);
		GX_TexCoord2u8(0,1);
	}

	GX_End();*/
}
