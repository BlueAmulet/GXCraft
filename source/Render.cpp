#include <grrlib.h>
#include <cstdarg>

#include "Render.hpp"
#include "Main.hpp"
#include "DisplayList.hpp"

static char dest[41];

DisplayList* dlist;

void Render::bind(DisplayList* list) {
	dlist = list;
}

DisplayList* Render::getbound() {
	return dlist;
}

void Render::drawText(f32 xPos, f32 yPos, GRRLIB_texImg* font, const char* format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(dest, 40, format, argptr);
    va_end(argptr);
	GRRLIB_Printf(xPos+2, yPos+2, font, 0x0000007F, 1, "%s", dest);
	GRRLIB_Printf(xPos  , yPos  , font, 0xFFFFFFFF, 1, "%s", dest);
}

bool Render::testFace(u8 face) {
	return face == 0 || face == 6 || face == 8 || face == 18 || face == 20 || face == 37 || face == 38 || face == 39 || face == 40;
}

void Render::drawBlock(int xPos, int yPos, int zPos, blockTexture *tex) {
	bool drawBack  = zPos >= worldZ-1 || testFace(theWorld->theWorld[yPos][xPos][zPos + 1]);
	bool drawFront = zPos <= 0        || testFace(theWorld->theWorld[yPos][xPos][zPos - 1]);
	bool drawRight = xPos >= worldX-1 || testFace(theWorld->theWorld[yPos][xPos + 1][zPos]);
	bool drawLeft  = xPos <= 0        || testFace(theWorld->theWorld[yPos][xPos - 1][zPos]);
	bool drawTop   = yPos >= worldY-1 || testFace(theWorld->theWorld[yPos + 1][xPos][zPos]);
	bool drawBott  = yPos > 0         && testFace(theWorld->theWorld[yPos - 1][xPos][zPos]);

	if (!drawBack && !drawFront && !drawRight && !drawLeft && !drawTop && !drawBott) return;

	f32 u0 = tex->u0, v0 = tex->v0;
	f32 u1 = tex->u1, v1 = tex->v1;

	if (drawBack) {
		dlist->add(xPos, 1+yPos, 1+zPos,
		0xCCCF,
		u0,v0);
		dlist->add( 1+xPos, 1+yPos, 1+zPos,
		0xCCCF,
		u1,v0);
		dlist->add( 1+xPos,yPos, 1+zPos,
		0xCCCF,
		u1,v1);
		dlist->add(xPos,yPos, 1+zPos,
		0xCCCF,
		u0,v1);
	}

	if (drawFront) {
		dlist->add( 1+xPos, 1+yPos,zPos,
		0xCCCF,
		u0,v0);
		dlist->add(xPos, 1+yPos,zPos,
		0xCCCF,
		u1,v0);
		dlist->add(xPos,yPos,zPos,
		0xCCCF,
		u1,v1);
		dlist->add( 1+xPos,yPos,zPos,
		0xCCCF,
		u0,v1);
	}

	if (drawRight) {
		dlist->add( 1+xPos, 1+yPos, 1+zPos,
		0x999F,
		u0,v0);
		dlist->add( 1+xPos, 1+yPos,zPos,
		0x999F,
		u1,v0);
		dlist->add( 1+xPos,yPos,zPos,
		0x999F,
		u1,v1);
		dlist->add( 1+xPos,yPos, 1+zPos,
		0x999F,
		u0,v1);
	}

	if (drawLeft) {
		dlist->add(xPos, 1+yPos,zPos,
		0x999F,
		u0,v0);
		dlist->add(xPos, 1+yPos, 1+zPos,
		0x999F,
		u1,v0);
		dlist->add(xPos,yPos, 1+zPos,
		0x999F,
		u1,v1);
		dlist->add(xPos,yPos,zPos,
		0x999F,
		u0,v1);
	}

	if (drawTop) {
		unsigned short c = theWorld->lighting[xPos][zPos] <= yPos ? 0xFFFF : 0x999F;
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

void Render::drawBlockCrossed(int xPos, int yPos, int zPos, blockTexture *tex) {
	unsigned short c = theWorld->lighting[xPos][zPos] <= yPos ? 0xFFFF : 0x999F;

	f32 u0 = tex->u0, v0 = tex->v0;
	f32 u1 = tex->u1, v1 = tex->v1;

	dlist->add(xPos, 1+yPos, zPos,
	c,
	u0,v0);
	dlist->add( 1+xPos, 1+yPos, 1+zPos,
	c,
	u1,v0);
	dlist->add( 1+xPos,yPos, 1+zPos,
	c,
	u1,v1);
	dlist->add(xPos,yPos,zPos,
	c,
	u0,v1);

	dlist->add(xPos, 1+yPos, 1+zPos,
	c,
	u0,v0);
	dlist->add( 1+xPos, 1+yPos,zPos,
	c,
	u1,v0);
	dlist->add( 1+xPos,yPos,zPos,
	c,
	u1,v1);
	dlist->add(xPos,yPos, 1+zPos,
	c,
	u0,v1);
}

void Render::drawMultiTexBlock(int xPos, int yPos, int zPos, blockTexture *texTop, blockTexture *texSide, blockTexture *texBott) {
	bool drawBack  = zPos >= worldZ-1 || testFace(theWorld->theWorld[yPos][xPos][zPos + 1]);
	bool drawFront = zPos <= 0        || testFace(theWorld->theWorld[yPos][xPos][zPos - 1]);
	bool drawRight = xPos >= worldX-1 || testFace(theWorld->theWorld[yPos][xPos + 1][zPos]);
	bool drawLeft  = xPos <= 0        || testFace(theWorld->theWorld[yPos][xPos - 1][zPos]);
	bool drawTop   = yPos >= worldY-1 || testFace(theWorld->theWorld[yPos + 1][xPos][zPos]);
	bool drawBott  = yPos > 0         && testFace(theWorld->theWorld[yPos - 1][xPos][zPos]);

	if (!drawBack && !drawFront && !drawRight && !drawLeft && !drawTop && !drawBott) return;

	f32 u0 = texSide->u0, v0 = texSide->v0;
	f32 u1 = texSide->u1, v1 = texSide->v1;

	if (drawBack) {
		dlist->add(xPos, 1+yPos, 1+zPos,
		0xCCCF,
		u0,v0);
		dlist->add( 1+xPos, 1+yPos, 1+zPos,
		0xCCCF,
		u1,v0);
		dlist->add( 1+xPos,yPos, 1+zPos,
		0xCCCF,
		u1,v1);
		dlist->add(xPos,yPos, 1+zPos,
		0xCCCF,
		u0,v1);
	}

	if (drawFront) {
		dlist->add( 1+xPos, 1+yPos,zPos,
		0xCCCF,
		u0,v0);
		dlist->add(xPos, 1+yPos,zPos,
		0xCCCF,
		u1,v0);
		dlist->add(xPos,yPos,zPos,
		0xCCCF,
		u1,v1);
		dlist->add( 1+xPos,yPos,zPos,
		0xCCCF,
		u0,v1);
	}

	if (drawRight) {
		dlist->add( 1+xPos, 1+yPos, 1+zPos,
		0x999F,
		u0,v0);
		dlist->add( 1+xPos, 1+yPos,zPos,
		0x999F,
		u1,v0);
		dlist->add( 1+xPos,yPos,zPos,
		0x999F,
		u1,v1);
		dlist->add( 1+xPos,yPos, 1+zPos,
		0x999F,
		u0,v1);
	}

	if (drawLeft) {
		dlist->add(xPos, 1+yPos,zPos,
		0x999F,
		u0,v0);
		dlist->add(xPos, 1+yPos, 1+zPos,
		0x999F,
		u1,v0);
		dlist->add(xPos,yPos, 1+zPos,
		0x999F,
		u1,v1);
		dlist->add(xPos,yPos,zPos,
		0x999F,
		u0,v1);
	}

	if (drawTop) {
		u0 = texTop->u0, v0 = texTop->v0;
		u1 = texTop->u1, v1 = texTop->v1;

		unsigned short c = theWorld->lighting[xPos][zPos] <= yPos ? 0xFFFF : 0x999F;

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
		u0 = texTop->u0, v0 = texTop->v0;
		u1 = texTop->u1, v1 = texTop->v1;

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

void Render::drawSelectionBlock(int xPos, int yPos, int zPos) {
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
