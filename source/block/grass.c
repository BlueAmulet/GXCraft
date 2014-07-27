#include <grrlib.h>

#include "../main.h"
#include "../block.h"
#include "../render.h"
#include "../textures/block_grass_side.h"
#include "../textures/block_grass_top.h"
#include "../textures/block_dirt.h"
#include "grass.h"

GRRLIB_texImg *tex_grass_side;
GRRLIB_texImg *tex_grass_top;
GRRLIB_texImg *tex_dirt;

inline void drawGrassBlock(int xPos, int yPos, int zPos);

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawGrassBlock(xPos, yPos, zPos);
}

void grass_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(2, entry);
	tex_grass_side = GRRLIB_LoadTexture(block_grass_side);
	tex_grass_top = GRRLIB_LoadTexture(block_grass_top);
	tex_dirt = GRRLIB_LoadTexture(block_dirt);
}

void grass_clean() {
	GRRLIB_FreeTexture(tex_grass_side);
	GRRLIB_FreeTexture(tex_grass_top);
	GRRLIB_FreeTexture(tex_dirt);
}

inline void drawGrassBlock(int xPos, int yPos, int zPos) {

	bool drawBack  = zPos >= worldZ-1 || testFace(theWorld[yPos][xPos][zPos + 1]);
	bool drawFront = zPos <= 0        || testFace(theWorld[yPos][xPos][zPos - 1]);
	bool drawRight = xPos >= worldX-1 || testFace(theWorld[yPos][xPos + 1][zPos]);
	bool drawLeft  = xPos <= 0        || testFace(theWorld[yPos][xPos - 1][zPos]);
	bool drawTop   = yPos >= worldY-1 || testFace(theWorld[yPos + 1][xPos][zPos]);
	bool drawBott  = yPos > 0         && testFace(theWorld[yPos - 1][xPos][zPos]);

	int size = 0;
	if (drawBack)  size += 4;
	if (drawFront) size += 4;
	if (drawRight) size += 4;
	if (drawLeft)  size += 4;
	
	if (size == 0 && !drawTop && !drawBott) return;
	
	if ((drawBack || drawFront || drawLeft || drawRight) && lastTex != tex_grass_side) {
		GRRLIB_SetTexture(tex_grass_side, 0);
		lastTex = tex_grass_side;
	}

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

	GX_End();

	if (drawTop) {
		if (lastTex != tex_grass_top) {
			GRRLIB_SetTexture(tex_grass_top, 0);
			lastTex = tex_grass_top;
		}

		GX_Begin(GX_QUADS, GX_VTXFMT1, 4);

		GX_Position3s16(xPos, 1+yPos,zPos);
		GX_Color1u16(0xFFFF);
		GX_TexCoord2u8(0,0);
		GX_Position3s16( 1+xPos, 1+yPos,zPos);
		GX_Color1u16(0xFFFF);
		GX_TexCoord2u8(1,0);
		GX_Position3s16( 1+xPos, 1+yPos, 1+zPos);
		GX_Color1u16(0xFFFF);
		GX_TexCoord2u8(1,1);
		GX_Position3s16(xPos, 1+yPos, 1+zPos);
		GX_Color1u16(0xFFFF);
		GX_TexCoord2u8(0,1);

		GX_End();
	}

	if (drawBott) {
		if (lastTex != tex_dirt) {
			GRRLIB_SetTexture(tex_dirt, 0);
			lastTex = tex_dirt;
		}

		GX_Begin(GX_QUADS, GX_VTXFMT1, 4);

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

		GX_End();
	}
}
