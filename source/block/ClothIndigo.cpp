#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothIndigo.hpp"

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
