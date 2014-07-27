#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_violet.h"
#include "cloth_violet.h"

GRRLIB_texImg *tex_cloth_violet;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_violet);
}

void cloth_violet_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(31, entry);
	tex_cloth_violet = GRRLIB_LoadTexture(block_cloth_violet);
}

void cloth_violet_clean() {
	GRRLIB_FreeTexture(tex_cloth_violet);
}
