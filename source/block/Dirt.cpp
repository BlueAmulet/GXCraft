#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Dirt.hpp"

static blockTexture *tex_dirt;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_dirt);
}

void dirt_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(3, entry);
	tex_dirt = getTexture(2, 0);
}
