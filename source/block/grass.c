#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_grass_side.h"
#include "../textures/block_grass_top.h"
#include "../textures/block_dirt.h"
#include "grass.h"

GRRLIB_texImg *tex_grass_side;
GRRLIB_texImg *tex_grass_top;
GRRLIB_texImg *tex_dirt;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_grass_top, 0);
	drawBlock(blockID, xPos, yPos, zPos);
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
