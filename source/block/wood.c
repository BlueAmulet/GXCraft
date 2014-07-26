#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_wood.h"
#include "wood.h"

GRRLIB_texImg *tex_wood;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_wood, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void wood_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(5, entry);
	tex_wood = GRRLIB_LoadTexture(block_wood);
}

void wood_clean() {
	GRRLIB_FreeTexture(tex_wood);
}
