#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_lime.h"
#include "cloth_lime.h"

GRRLIB_texImg *tex_cloth_lime;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_lime, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_lime_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(24, entry);
	tex_cloth_lime = GRRLIB_LoadTexture(block_cloth_lime);
}

void cloth_lime_clean() {
	GRRLIB_FreeTexture(tex_cloth_lime);
}
