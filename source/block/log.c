#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_log_top.h"
#include "../textures/block_log_side.h"
#include "log.h"

GRRLIB_texImg *tex_log_top;
GRRLIB_texImg *tex_log_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_log_top);
}

void log_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(17, entry);
	tex_log_top = GRRLIB_LoadTexture(block_log_top);
	tex_log_side = GRRLIB_LoadTexture(block_log_side);
}

void log_clean() {
	GRRLIB_FreeTexture(tex_log_top);
	GRRLIB_FreeTexture(tex_log_side);
}
