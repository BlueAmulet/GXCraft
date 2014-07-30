#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "dirt.h"

blockTexture *tex_dirt;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_dirt);
}

void dirt_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(3, entry);
	tex_dirt = getTexture(2, 0);
}
