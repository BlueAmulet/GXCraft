#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "glass.hpp"

static blockTexture *tex_glass;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	Render::drawBlock(xPos, yPos, zPos, tex_glass);
}

void glass_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(20, entry);
	tex_glass = getTexture(1, 3);
}
