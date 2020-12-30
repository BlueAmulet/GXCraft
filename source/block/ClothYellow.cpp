#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothYellow.hpp"

static blockTexture *tex_cloth_yellow;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_yellow);
}

void cloth_yellow_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(23, entry);
	tex_cloth_yellow = getTexture(2, 4);
}
