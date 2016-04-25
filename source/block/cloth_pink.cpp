#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "cloth_pink.hpp"

static blockTexture *tex_cloth_pink;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_pink);
}

void cloth_pink_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(33, entry);
	tex_cloth_pink = getTexture(11, 4);
}
