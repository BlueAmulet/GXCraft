#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_black.h"
#include "cloth_black.h"

GRRLIB_texImg *tex_cloth_black;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_black, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_black_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(34, entry);
	tex_cloth_black = GRRLIB_LoadTexture(block_cloth_black);
}

void cloth_black_clean() {
	GRRLIB_FreeTexture(tex_cloth_black);
}
