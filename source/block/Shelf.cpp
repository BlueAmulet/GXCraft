#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Shelf.hpp"

static blockTexture *tex_shelf;
static blockTexture *tex_wood;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawMultiTexBlock(xPos, yPos, zPos, tex_wood, tex_shelf, tex_wood);
}

void shelf_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(47, entry);
	tex_shelf = getTexture(3, 2);
	tex_wood = getTexture(4, 0);
}
