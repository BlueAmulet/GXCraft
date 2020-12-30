#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Log.hpp"

static blockTexture *tex_log_top;
static blockTexture *tex_log_side;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawMultiTexBlock(xPos, yPos, zPos, tex_log_top, tex_log_side, tex_log_top);
}

void log_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(17, entry);
	tex_log_top = getTexture(5, 1);
	tex_log_side = getTexture(4, 1);
}
