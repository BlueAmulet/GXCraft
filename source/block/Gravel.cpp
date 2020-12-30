#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Gravel.hpp"

static blockTexture *tex_gravel;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_gravel);
}

void gravel_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(13, entry);
	tex_gravel = getTexture(3, 1);
}
