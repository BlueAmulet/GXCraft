#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_indigo.h"
#include "cloth_indigo.h"

GRRLIB_texImg *tex_cloth_indigo;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_indigo, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_indigo_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(30, entry);
	tex_cloth_indigo = GRRLIB_LoadTexture(block_cloth_indigo);
}

void cloth_indigo_clean() {
	GRRLIB_FreeTexture(tex_cloth_indigo);
}
