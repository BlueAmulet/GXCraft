#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_lava.h"
#include "lava.h"

GRRLIB_texImg *tex_lava;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_lava);
}

void lava_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(10, entry);
	tex_lava = GRRLIB_LoadTexture(block_lava);
}

void lava_clean() {
	GRRLIB_FreeTexture(tex_lava);
}
