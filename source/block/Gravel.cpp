#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Gravel.hpp"

static blockTexture *tex_gravel;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_gravel);
}

void gravel_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(13, entry);
	tex_gravel = getTexture(3, 1);
}
