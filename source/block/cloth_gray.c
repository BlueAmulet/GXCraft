#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_gray.h"
#include "cloth_gray.h"

GRRLIB_texImg *tex_cloth_gray;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_gray, 0);
	drawBlock(blockID, xPos, yPos, zPos);
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
