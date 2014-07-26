#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_mossy.h"
#include "mossy.h"

GRRLIB_texImg *tex_mossy;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_mossy, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void mossy_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(48, entry);
	tex_mossy = GRRLIB_LoadTexture(block_mossy);
}

void mossy_clean() {
	GRRLIB_FreeTexture(tex_mossy);
}
