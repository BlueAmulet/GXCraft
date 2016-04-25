#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "cloth_magenta.hpp"

static blockTexture *tex_cloth_magenta;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_magenta);
}

void cloth_magenta_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(32, entry);
	tex_cloth_magenta = getTexture(12, 4);
}
