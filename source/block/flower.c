#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_flower.h"
#include "flower.h"

GRRLIB_texImg *tex_flower;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_flower, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void flower_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(37, entry);
	tex_flower = GRRLIB_LoadTexture(block_flower);
}

void flower_clean() {
	GRRLIB_FreeTexture(tex_flower);
}
