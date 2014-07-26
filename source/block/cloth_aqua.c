#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_aqua.h"
#include "cloth_aqua.h"

GRRLIB_texImg *tex_cloth_aqua;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_aqua, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_aqua_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(26, entry);
	tex_cloth_aqua = GRRLIB_LoadTexture(block_cloth_aqua);
}

void cloth_aqua_clean() {
	GRRLIB_FreeTexture(tex_cloth_aqua);
}
