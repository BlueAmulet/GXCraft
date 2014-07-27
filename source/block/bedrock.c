#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_bedrock.h"
#include "bedrock.h"

GRRLIB_texImg *tex_bedrock;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_bedrock);
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
