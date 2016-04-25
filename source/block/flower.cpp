#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "flower.hpp"

static blockTexture *tex_flower;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlockCrossed(xPos, yPos, zPos, tex_flower);
}

void flower_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(37, entry);
	tex_flower = getTexture(13, 0);
}
