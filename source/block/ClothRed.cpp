#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothRed.hpp"

static blockTexture *tex_cloth_red;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_red);
}

void cloth_red_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(21, entry);
	tex_cloth_red = getTexture(0, 4);
}