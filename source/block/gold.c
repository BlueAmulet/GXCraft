#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_gold_top.h"
#include "../textures/block_gold_bott.h"
#include "../textures/block_gold_side.h"
#include "gold.h"

GRRLIB_texImg *tex_gold_top;
GRRLIB_texImg *tex_gold_bott;
GRRLIB_texImg *tex_gold_side;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_gold_top, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void gold_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(41, entry);
	tex_gold_top = GRRLIB_LoadTexture(block_gold_top);
	tex_gold_bott = GRRLIB_LoadTexture(block_gold_bott);
	tex_gold_side = GRRLIB_LoadTexture(block_gold_side);
}

void gold_clean() {
	GRRLIB_FreeTexture(tex_gold_top);
	GRRLIB_FreeTexture(tex_gold_bott);
	GRRLIB_FreeTexture(tex_gold_side);
}
