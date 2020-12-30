#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothBlue.hpp"

static blockTexture *tex_cloth_blue;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_blue);
}

void cloth_blue_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(28, entry);
	tex_cloth_blue = getTexture(7, 4);
}
