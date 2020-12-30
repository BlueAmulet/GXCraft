#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Wood.hpp"

static blockTexture *tex_wood;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_wood);
}

void wood_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(5, entry);
	tex_wood = getTexture(4, 0);
}
