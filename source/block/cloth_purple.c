#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_purple.h"
#include "cloth_purple.h"

GRRLIB_texImg *tex_cloth_purple;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_purple, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_purple_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(29, entry);
	tex_cloth_purple = GRRLIB_LoadTexture(block_cloth_purple);
}

void cloth_purple_clean() {
	GRRLIB_FreeTexture(tex_cloth_purple);
}
