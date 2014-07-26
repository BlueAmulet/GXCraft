#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_brick.h"
#include "brick.h"

GRRLIB_texImg *tex_brick;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_brick, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void brick_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(45, entry);
	tex_brick = GRRLIB_LoadTexture(block_brick);
}

void brick_clean() {
	GRRLIB_FreeTexture(tex_brick);
}
