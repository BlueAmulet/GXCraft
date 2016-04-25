#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "gold.h"

blockTexture *tex_gold_top;
blockTexture *tex_gold_bott;
blockTexture *tex_gold_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawMultiTexBlock(xPos, yPos, zPos, tex_gold_top, tex_gold_side, tex_gold_bott);
}

void gold_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(41, entry);
	tex_gold_top = getTexture(8, 1);
	tex_gold_bott = getTexture(8, 3);
	tex_gold_side = getTexture(8, 2);
}
