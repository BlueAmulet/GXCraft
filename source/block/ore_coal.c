#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_ore_coal.h"
#include "ore_coal.h"

GRRLIB_texImg *tex_ore_coal;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_ore_coal, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void ore_coal_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(16, entry);
	tex_ore_coal = GRRLIB_LoadTexture(block_ore_coal);
}

void ore_coal_clean() {
	GRRLIB_FreeTexture(tex_ore_coal);
}
