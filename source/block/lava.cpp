#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "lava.hpp"

static blockTexture *tex_lava;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_lava);
}

void lava_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(10, entry);
	tex_lava = getTexture(14, 1);
}
