#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_rose.h"
#include "rose.h"

GRRLIB_texImg *tex_rose;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_rose, 0);
	drawBlockCrossed(blockID, xPos, yPos, zPos);
}

void rose_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(38, entry);
	tex_rose = GRRLIB_LoadTexture(block_rose);
}

void rose_clean() {
	GRRLIB_FreeTexture(tex_rose);
}
