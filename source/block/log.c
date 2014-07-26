#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_log_top.h"
#include "../textures/block_log_side.h"
#include "log.h"

GRRLIB_texImg *tex_log_top;
GRRLIB_texImg *tex_log_side;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_log_top, 0);
	drawBlock(blockID, xPos, yPos, zPos);
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
