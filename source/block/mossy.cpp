#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "mossy.hpp"

static blockTexture *tex_mossy;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_mossy);
}

void mossy_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(48, entry);
	tex_mossy = getTexture(4, 2);
}
