#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_water.h"
#include "water.h"

GRRLIB_texImg *tex_water;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlock(xPos, yPos, zPos, tex_water);
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
