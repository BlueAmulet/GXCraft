#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_orange.h"
#include "cloth_orange.h"

GRRLIB_texImg *tex_cloth_orange;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_orange);
}

void cloth_orange_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(22, entry);
	tex_cloth_orange = GRRLIB_LoadTexture(block_cloth_orange);
}

void cloth_orange_clean() {
	GRRLIB_FreeTexture(tex_cloth_orange);
}
