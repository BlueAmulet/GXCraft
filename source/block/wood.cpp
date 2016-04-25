#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "wood.hpp"

static blockTexture *tex_wood;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_wood);
}

void wood_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(5, entry);
	tex_wood = getTexture(4, 0);
}
