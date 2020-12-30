#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothBlack.hpp"

static blockTexture *tex_cloth_black;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_black);
}

void cloth_black_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(34, entry);
	tex_cloth_black = getTexture(13, 4);
}
