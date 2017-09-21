#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothAqua.hpp"

static blockTexture *tex_cloth_aqua;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_aqua);
}

void cloth_aqua_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(26, entry);
	tex_cloth_aqua = getTexture(5, 4);
}
