#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Sand.hpp"

static blockTexture *tex_sand;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_sand);
}

void sand_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(12, entry);
	tex_sand = getTexture(2, 1);
}
