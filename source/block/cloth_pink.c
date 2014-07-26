#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_pink.h"
#include "cloth_pink.h"

GRRLIB_texImg *tex_cloth_pink;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_pink, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_pink_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(33, entry);
	tex_cloth_pink = GRRLIB_LoadTexture(block_cloth_pink);
}

void cloth_pink_clean() {
	GRRLIB_FreeTexture(tex_cloth_pink);
}
