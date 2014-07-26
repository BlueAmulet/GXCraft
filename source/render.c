#include <grrlib.h>

#include "render.h"
#include "main.h"

unsigned char lastID = 255;

static bool testFace(unsigned char face) {
	return face == 0 || face == 6 || face == 8 || face == 37 || face == 38 || face == 39 || face == 40;
}

void drawBlock(unsigned char blockID, int xPos, int yPos, int zPos) {

	lastID = blockID;

	int drawBack  = testFace(theWorld[yPos][xPos][zPos + 1]);
	int drawFront = testFace(theWorld[yPos][xPos][zPos - 1]);
	int drawRight = testFace(theWorld[yPos][xPos + 1][zPos]);
	int drawLeft  = testFace(theWorld[yPos][xPos - 1][zPos]);
	int drawTop   = testFace(theWorld[yPos + 1][xPos][zPos]);
	int drawBott  = testFace(theWorld[yPos - 1][xPos][zPos]);

	int size = 0;
	if (drawBack)  size += 4;
	if (drawFront) size += 4;
	if (drawRight) size += 4;
	if (drawLeft)  size += 4;
	if (drawTop)   size += 4;
	if (drawBott)  size += 4;
	
	if (size == 0) return;

	GX_Begin(GX_QUADS, GX_VTXFMT0, size);

	if (drawBack) {
		GX_Position3f32(-0.5f+xPos, 0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0xCCCCCCFF);
		GX_TexCoord2f32(0.0f,0.0f);
		GX_Position3f32( 0.5f+xPos, 0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0xCCCCCCFF);
		GX_TexCoord2f32(1.0f,0.0f);
		GX_Position3f32( 0.5f+xPos,-0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0xCCCCCCFF);
		GX_TexCoord2f32(1.0f,1.0f);
		GX_Position3f32(-0.5f+xPos,-0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0xCCCCCCFF);
		GX_TexCoord2f32(0.0f,1.0f);
	}

	if (drawFront) {
		GX_Position3f32( 0.5f+xPos, 0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0xCCCCCCFF);
		GX_TexCoord2f32(0.0f,0.0f);
		GX_Position3f32(-0.5f+xPos, 0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0xCCCCCCFF);
		GX_TexCoord2f32(1.0f,0.0f);
		GX_Position3f32(-0.5f+xPos,-0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0xCCCCCCFF);
		GX_TexCoord2f32(1.0f,1.0f);
		GX_Position3f32( 0.5f+xPos,-0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0xCCCCCCFF);
		GX_TexCoord2f32(0.0f,1.0f);
	}

	if (drawRight) {
		GX_Position3f32( 0.5f+xPos, 0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0x999999FF);
		GX_TexCoord2f32(0.0f,0.0f);
		GX_Position3f32( 0.5f+xPos, 0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0x999999FF);
		GX_TexCoord2f32(1.0f,0.0f);
		GX_Position3f32( 0.5f+xPos,-0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0x999999FF);
		GX_TexCoord2f32(1.0f,1.0f);
		GX_Position3f32( 0.5f+xPos,-0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0x999999FF);
		GX_TexCoord2f32(0.0f,1.0f);
	}

	if (drawLeft) {
		GX_Position3f32(-0.5f+xPos, 0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0x999999FF);
		GX_TexCoord2f32(0.0f,0.0f);
		GX_Position3f32(-0.5f+xPos, 0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0x999999FF);
		GX_TexCoord2f32(1.0f,0.0f);
		GX_Position3f32(-0.5f+xPos,-0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0x999999FF);
		GX_TexCoord2f32(1.0f,1.0f);
		GX_Position3f32(-0.5f+xPos,-0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0x999999FF);
		GX_TexCoord2f32(0.0f,1.0f);
	}

	if (drawTop) {
		GX_Position3f32(-0.5f+xPos, 0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(0.0f,0.0f);
		GX_Position3f32( 0.5f+xPos, 0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(1.0f,0.0f);
		GX_Position3f32( 0.5f+xPos, 0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(1.0f,1.0f);
		GX_Position3f32(-0.5f+xPos, 0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(0.0f,1.0f);
	}

	if (drawBott) {
		GX_Position3f32( 0.5f+xPos,-0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0x4D4D4DFF);
		GX_TexCoord2f32(0.0f,0.0f);
		GX_Position3f32(-0.5f+xPos,-0.5f+yPos,-0.5f+zPos);
		GX_Color1u32(0x4D4D4DFF);
		GX_TexCoord2f32(1.0f,0.0f);
		GX_Position3f32(-0.5f+xPos,-0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0x4D4D4DFF);
		GX_TexCoord2f32(1.0f,1.0f);
		GX_Position3f32( 0.5f+xPos,-0.5f+yPos, 0.5f+zPos);
		GX_Color1u32(0x4D4D4DFF);
		GX_TexCoord2f32(0.0f,1.0f);
	}

	GX_End();
}

void drawBlockCrossed(unsigned char blockID, int xPos, int yPos, int zPos) {

	lastID = blockID;

	GX_Begin(GX_QUADS, GX_VTXFMT0, 8);

	GX_Position3f32(-0.5f+xPos, 0.5f+yPos, -0.5f+zPos);
	GX_Color1u32(0xFFFFFFFF);
	GX_TexCoord2f32(0.0f,0.0f);
	GX_Position3f32( 0.5f+xPos, 0.5f+yPos, 0.5f+zPos);
	GX_Color1u32(0xFFFFFFFF);
	GX_TexCoord2f32(1.0f,0.0f);
	GX_Position3f32( 0.5f+xPos,-0.5f+yPos, 0.5f+zPos);
	GX_Color1u32(0xFFFFFFFF);
	GX_TexCoord2f32(1.0f,1.0f);
	GX_Position3f32(-0.5f+xPos,-0.5f+yPos,-0.5f+zPos);
	GX_Color1u32(0xFFFFFFFF);
	GX_TexCoord2f32(0.0f,1.0f);

	GX_Position3f32(-0.5f+xPos, 0.5f+yPos, 0.5f+zPos);
	GX_Color1u32(0xFFFFFFFF);
	GX_TexCoord2f32(0.0f,0.0f);
	GX_Position3f32( 0.5f+xPos, 0.5f+yPos,-0.5f+zPos);
	GX_Color1u32(0xFFFFFFFF);
	GX_TexCoord2f32(1.0f,0.0f);
	GX_Position3f32( 0.5f+xPos,-0.5f+yPos,-0.5f+zPos);
	GX_Color1u32(0xFFFFFFFF);
	GX_TexCoord2f32(1.0f,1.0f);
	GX_Position3f32(-0.5f+xPos,-0.5f+yPos, 0.5f+zPos);
	GX_Color1u32(0xFFFFFFFF);
	GX_TexCoord2f32(0.0f,1.0f);

	GX_End();
}
