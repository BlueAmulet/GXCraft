#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_water.h"
#include "water.h"

GRRLIB_texImg *tex_water;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_water, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void water_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(8, entry);
	tex_water = GRRLIB_LoadTexture(block_water);
}

void water_clean() {
	GRRLIB_FreeTexture(tex_water);
}
