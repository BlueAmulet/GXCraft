#include <grrlib.h>

#include "render.h"
#include "main.h"

GRRLIB_texImg *lastTex = NULL;

bool testFace(unsigned char face) {
	return face == 0 || face == 6 || face == 8 || face == 18 || face == 37 || face == 38 || face == 39 || face == 40;
}

inline void drawBlock(int xPos, int yPos, int zPos, GRRLIB_texImg *tex) {

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

	GX_End();
}

inline void drawBlockCrossed(int xPos, int yPos, int zPos, GRRLIB_texImg *tex) {

	if (lastTex != tex)
		GRRLIB_SetTexture(tex, 0);
	lastTex = tex;

	GX_Begin(GX_QUADS, GX_VTXFMT1, 8);

	unsigned short c = lighting[xPos][zPos] <= yPos ? 0xFFFF : 0x999F;

	GX_Position3s16(xPos, 1+yPos, zPos);
	GX_Color1u16(c);
	GX_TexCoord2u8(0,0);
	GX_Position3s16( 1+xPos, 1+yPos, 1+zPos);
	GX_Color1u16(c);
	GX_TexCoord2u8(1,0);
	GX_Position3s16( 1+xPos,yPos, 1+zPos);
	GX_Color1u16(c);
	GX_TexCoord2u8(1,1);
	GX_Position3s16(xPos,yPos,zPos);
	GX_Color1u16(c);
	GX_TexCoord2u8(0,1);

	GX_Position3s16(xPos, 1+yPos, 1+zPos);
	GX_Color1u16(c);
	GX_TexCoord2u8(0,0);
	GX_Position3s16( 1+xPos, 1+yPos,zPos);
	GX_Color1u16(c);
	GX_TexCoord2u8(1,0);
	GX_Position3s16( 1+xPos,yPos,zPos);
	GX_Color1u16(c);
	GX_TexCoord2u8(1,1);
	GX_Position3s16(xPos,yPos, 1+zPos);
	GX_Color1u16(c);
	GX_TexCoord2u8(0,1);

	GX_End();
}

inline void drawMultiTexBlock(int xPos, int yPos, int zPos, GRRLIB_texImg *texTop, GRRLIB_texImg *texSide, GRRLIB_texImg *texBott) {

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
	
	if ((drawBack || drawFront || drawLeft || drawRight) && lastTex != texSide) {
		GRRLIB_SetTexture(texSide, 0);
		lastTex = texSide;
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
		if (lastTex != texTop) {
			GRRLIB_SetTexture(texTop, 0);
			lastTex = texTop;
		}

		unsigned short c = lighting[xPos][zPos] <= yPos ? 0xFFFF : 0x999F;

		GX_Begin(GX_QUADS, GX_VTXFMT1, 4);

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

		GX_End();
	}

	if (drawBott) {
		if (lastTex != texBott) {
			GRRLIB_SetTexture(texBott, 0);
			lastTex = texBott;
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

inline void drawSelectionBlock(int xPos, int yPos, int zPos) {

	GX_Begin(GX_LINES, GX_VTXFMT0, 24);

	GX_Position3f32(xPos-0.01,yPos+1.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos+1.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos+1.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos-0.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);

	GX_Position3f32(xPos+1.01,yPos-0.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos-0.01,yPos-0.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos-0.01,yPos-0.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos-0.01,yPos+1.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);

	GX_Position3f32(xPos-0.01,yPos+1.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos+1.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos+1.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos-0.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);

	GX_Position3f32(xPos+1.01,yPos-0.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos-0.01,yPos-0.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos-0.01,yPos-0.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos-0.01,yPos+1.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);

	GX_Position3f32(xPos-0.01,yPos+1.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos-0.01,yPos+1.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos+1.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos+1.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);

	GX_Position3f32(xPos-0.01,yPos-0.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos-0.01,yPos-0.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos-0.01,zPos-0.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);
	GX_Position3f32(xPos+1.01,yPos-0.01,zPos+1.01);
	GX_Color1u32(0x000000CC);
	GX_TexCoord2f32(0,0);

	GX_End();
}
