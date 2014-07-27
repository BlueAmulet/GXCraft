#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_purple.h"
#include "cloth_purple.h"

GRRLIB_texImg *tex_cloth_purple;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_purple);
}

void cloth_purple_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(29, entry);
	tex_cloth_purple = GRRLIB_LoadTexture(block_cloth_purple);
}

void cloth_purple_clean() {
	GRRLIB_FreeTexture(tex_cloth_purple);
}
