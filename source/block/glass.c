#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_glass.h"
#include "glass.h"

GRRLIB_texImg *tex_glass;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlock(xPos, yPos, zPos, tex_glass);
}

void glass_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(20, entry);
	tex_glass = GRRLIB_LoadTexture(block_glass);
}

void glass_clean() {
	GRRLIB_FreeTexture(tex_glass);
}
