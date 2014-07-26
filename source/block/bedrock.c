#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_bedrock.h"
#include "bedrock.h"

GRRLIB_texImg *tex_bedrock;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_bedrock, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void bedrock_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(7, entry);
	tex_bedrock = GRRLIB_LoadTexture(block_bedrock);
}

void bedrock_clean() {
	GRRLIB_FreeTexture(tex_bedrock);
}
