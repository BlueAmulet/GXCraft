#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "cloth_indigo.hpp"

static blockTexture *tex_cloth_indigo;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_indigo);
}

void cloth_indigo_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(30, entry);
	tex_cloth_indigo = getTexture(8, 4);
}
