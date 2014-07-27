#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_red.h"
#include "cloth_red.h"

GRRLIB_texImg *tex_cloth_red;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_red);
}

void cloth_red_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(21, entry);
	tex_cloth_red = GRRLIB_LoadTexture(block_cloth_red);
}

void cloth_red_clean() {
	GRRLIB_FreeTexture(tex_cloth_red);
}
