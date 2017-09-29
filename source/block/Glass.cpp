#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Glass.hpp"

static blockTexture *tex_glass;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_glass);
}

void glass_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(20, entry);
	tex_glass = getTexture(1, 3);
}
