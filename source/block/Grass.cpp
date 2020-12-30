#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Grass.hpp"

static blockTexture *tex_grass_side;
static blockTexture *tex_grass_top;
static blockTexture *tex_dirt;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawMultiTexBlock(xPos, yPos, zPos, tex_grass_top, tex_grass_side, tex_dirt);
}

void grass_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(2, entry);
	tex_grass_side = getTexture(3, 0);
	tex_grass_top = getTexture(0, 0);
	tex_dirt = getTexture(2, 0);
}
