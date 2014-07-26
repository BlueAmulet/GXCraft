#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_slab_top.h"
#include "../textures/block_slab_side.h"
#include "slab.h"

GRRLIB_texImg *tex_slab_top;
GRRLIB_texImg *tex_slab_side;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_slab_top, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void slab_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(43, entry);
	tex_slab_top = GRRLIB_LoadTexture(block_slab_top);
	tex_slab_side = GRRLIB_LoadTexture(block_slab_side);
}

void slab_clean() {
	GRRLIB_FreeTexture(tex_slab_top);
	GRRLIB_FreeTexture(tex_slab_side);
}
