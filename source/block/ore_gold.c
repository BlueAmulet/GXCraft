#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_ore_gold.h"
#include "ore_gold.h"

GRRLIB_texImg *tex_ore_gold;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_ore_gold, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void ore_gold_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(14, entry);
	tex_ore_gold = GRRLIB_LoadTexture(block_ore_gold);
}

void ore_gold_clean() {
	GRRLIB_FreeTexture(tex_ore_gold);
}
