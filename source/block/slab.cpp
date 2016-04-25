#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "slab.hpp"

static blockTexture *tex_slab;
static blockTexture *tex_slab_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawMultiTexBlock(xPos, yPos, zPos, tex_slab, tex_slab_side, tex_slab);
}

void slab_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(43, entry);
	tex_slab = getTexture(6, 0);
	tex_slab_side = getTexture(5, 0);
}
