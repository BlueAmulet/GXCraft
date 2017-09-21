#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothViolet.hpp"

static blockTexture *tex_cloth_violet;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_violet);
}

void cloth_violet_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(31, entry);
	tex_cloth_violet = getTexture(10, 4);
}
