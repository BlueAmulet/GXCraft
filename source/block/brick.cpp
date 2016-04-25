#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "brick.hpp"

static blockTexture *tex_brick;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_brick);
}

void brick_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(45, entry);
	tex_brick = getTexture(7, 0);
}
