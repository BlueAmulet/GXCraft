#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "bedrock.h"

blockTexture *tex_bedrock;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_bedrock);
}

void bedrock_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(7, entry);
	tex_bedrock = getTexture(1, 1);
}
