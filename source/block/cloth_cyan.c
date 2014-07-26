#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_cyan.h"
#include "cloth_cyan.h"

GRRLIB_texImg *tex_cloth_cyan;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_cyan, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_cyan_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(27, entry);
	tex_cloth_cyan = GRRLIB_LoadTexture(block_cloth_cyan);
}

void cloth_cyan_clean() {
	GRRLIB_FreeTexture(tex_cloth_cyan);
}
