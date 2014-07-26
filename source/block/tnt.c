#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_tnt_top.h"
#include "../textures/block_tnt_bott.h"
#include "../textures/block_tnt.h"
#include "tnt.h"

GRRLIB_texImg *tex_tnt_top;
GRRLIB_texImg *tex_tnt_bott;
GRRLIB_texImg *tex_tnt;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_tnt, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void tnt_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(46, entry);
	tex_tnt_top = GRRLIB_LoadTexture(block_tnt_top);
	tex_tnt_bott = GRRLIB_LoadTexture(block_tnt_bott);
	tex_tnt = GRRLIB_LoadTexture(block_tnt);
}

void tnt_clean() {
	GRRLIB_FreeTexture(tex_tnt_top);
	GRRLIB_FreeTexture(tex_tnt_bott);
	GRRLIB_FreeTexture(tex_tnt);
}
