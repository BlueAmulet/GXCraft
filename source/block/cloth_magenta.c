#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_magenta.h"
#include "cloth_magenta.h"

GRRLIB_texImg *tex_cloth_magenta;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_magenta, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_magenta_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(32, entry);
	tex_cloth_magenta = GRRLIB_LoadTexture(block_cloth_magenta);
}

void cloth_magenta_clean() {
	GRRLIB_FreeTexture(tex_cloth_magenta);
}
