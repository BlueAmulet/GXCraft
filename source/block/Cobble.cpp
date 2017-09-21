#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Cobble.hpp"

static blockTexture *tex_cobble;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cobble);
}

void cobble_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(4, entry);
	tex_cobble = getTexture(0, 1);
}
