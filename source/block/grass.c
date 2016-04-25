#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "grass.h"

blockTexture *tex_grass_side;
blockTexture *tex_grass_top;
blockTexture *tex_dirt;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawMultiTexBlock(xPos, yPos, zPos, tex_grass_top, tex_grass_side, tex_dirt);
}

void grass_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(2, entry);
	tex_grass_side = getTexture(3, 0);
	tex_grass_top = getTexture(0, 0);
	tex_dirt = getTexture(2, 0);
}

