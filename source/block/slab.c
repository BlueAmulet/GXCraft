#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_slab_top.h"
#include "../textures/block_slab_side.h"
#include "slab.h"

GRRLIB_texImg *tex_slab_top;
GRRLIB_texImg *tex_slab_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_slab_top);
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
