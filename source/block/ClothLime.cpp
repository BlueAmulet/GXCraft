#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothLime.hpp"

static blockTexture *tex_cloth_lime;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_lime);
}

void cloth_lime_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(24, entry);
	tex_cloth_lime = getTexture(3, 4);
}
