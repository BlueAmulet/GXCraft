#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Stone.hpp"

static blockTexture *tex_stone;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_stone);
}

void stone_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(1, entry);
	tex_stone = getTexture(1, 0);
}
