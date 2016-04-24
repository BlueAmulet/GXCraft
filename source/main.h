#pragma once

#define worldX 512
#define worldY 64
#define worldZ 512

#define chunkSize 16

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

extern GRRLIB_texImg *tex_blockicons[256];

extern u8 theWorld[worldY][worldX][worldZ];
extern u8 lighting[worldX][worldZ];
