#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "cloth_orange.hpp"

static blockTexture *tex_cloth_orange;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_orange);
}

void cloth_orange_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(22, entry);
	tex_cloth_orange = getTexture(1, 4);
}
