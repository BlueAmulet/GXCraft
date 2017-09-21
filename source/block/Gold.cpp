#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Gold.hpp"

static blockTexture *tex_gold_top;
static blockTexture *tex_gold_bott;
static blockTexture *tex_gold_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawMultiTexBlock(xPos, yPos, zPos, tex_gold_top, tex_gold_side, tex_gold_bott);
}

void gold_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(41, entry);
	tex_gold_top = getTexture(8, 1);
	tex_gold_bott = getTexture(8, 3);
	tex_gold_side = getTexture(8, 2);
}
