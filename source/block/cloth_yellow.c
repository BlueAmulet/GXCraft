#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_yellow.h"
#include "cloth_yellow.h"

GRRLIB_texImg *tex_cloth_yellow;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_yellow);
}

void cloth_yellow_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(23, entry);
	tex_cloth_yellow = GRRLIB_LoadTexture(block_cloth_yellow);
}

void cloth_yellow_clean() {
	GRRLIB_FreeTexture(tex_cloth_yellow);
}
