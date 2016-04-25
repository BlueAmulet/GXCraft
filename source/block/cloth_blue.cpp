#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "cloth_blue.hpp"

static blockTexture *tex_cloth_blue;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_blue);
}

void cloth_blue_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(28, entry);
	tex_cloth_blue = getTexture(7, 4);
}
