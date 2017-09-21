#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Rose.hpp"

static blockTexture *tex_rose;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlockCrossed(xPos, yPos, zPos, tex_rose);
}

void rose_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(38, entry);
	tex_rose = getTexture(12, 0);
}
