#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "log.hpp"

static blockTexture *tex_log_top;
static blockTexture *tex_log_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawMultiTexBlock(xPos, yPos, zPos, tex_log_top, tex_log_side, tex_log_top);
}

void log_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(17, entry);
	tex_log_top = getTexture(5, 1);
	tex_log_side = getTexture(4, 1);
}
