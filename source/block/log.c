#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "log.h"

blockTexture *tex_log_top;
blockTexture *tex_log_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawMultiTexBlock(xPos, yPos, zPos, tex_log_top, tex_log_side, tex_log_top);
}

void log_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(17, entry);
	tex_log_top = getTexture(5, 1);
	tex_log_side = getTexture(4, 1);
}
