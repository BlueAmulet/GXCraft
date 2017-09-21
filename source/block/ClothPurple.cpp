#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothPurple.hpp"

static blockTexture *tex_cloth_purple;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_purple);
}

void cloth_purple_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(29, entry);
	tex_cloth_purple = getTexture(9, 4);
}
