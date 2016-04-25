#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "cloth_white.hpp"

static blockTexture *tex_cloth_white;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_white);
}

void cloth_white_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(36, entry);
	tex_cloth_white = getTexture(15, 4);
}
