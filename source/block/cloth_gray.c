#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_gray.h"
#include "cloth_gray.h"

GRRLIB_texImg *tex_cloth_gray;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_gray);
}

void cloth_gray_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(35, entry);
	tex_cloth_gray = GRRLIB_LoadTexture(block_cloth_gray);
}

void cloth_gray_clean() {
	GRRLIB_FreeTexture(tex_cloth_gray);
}
