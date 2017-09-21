#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothGreen.hpp"

static blockTexture *tex_cloth_green;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_green);
}

void cloth_green_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(25, entry);
	tex_cloth_green = getTexture(4, 4);
}
