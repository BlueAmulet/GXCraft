#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_blue.h"
#include "cloth_blue.h"

GRRLIB_texImg *tex_cloth_blue;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_blue);
}

void cloth_blue_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(28, entry);
	tex_cloth_blue = GRRLIB_LoadTexture(block_cloth_blue);
}

void cloth_blue_clean() {
	GRRLIB_FreeTexture(tex_cloth_blue);
}
