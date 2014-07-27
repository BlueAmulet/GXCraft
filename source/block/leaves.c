#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_leaves.h"
#include "leaves.h"

GRRLIB_texImg *tex_leaves;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlock(xPos, yPos, zPos, tex_leaves);
}

void leaves_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(18, entry);
	tex_leaves = GRRLIB_LoadTexture(block_leaves);
}

void leaves_clean() {
	GRRLIB_FreeTexture(tex_leaves);
}
