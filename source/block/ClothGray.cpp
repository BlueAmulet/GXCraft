#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothGray.hpp"

static blockTexture *tex_cloth_gray;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_gray);
}

void cloth_gray_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(35, entry);
	tex_cloth_gray = getTexture(14, 4);
}
