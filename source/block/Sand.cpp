#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Sand.hpp"

static blockTexture *tex_sand;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_sand);
}

void sand_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(12, entry);
	tex_sand = getTexture(2, 1);
}
