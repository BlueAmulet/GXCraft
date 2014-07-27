#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_ore_gold.h"
#include "ore_gold.h"

GRRLIB_texImg *tex_ore_gold;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_ore_gold);
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
