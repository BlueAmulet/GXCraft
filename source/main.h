#include <grrlib.h>

#define worldX 512
#define worldY 64
#define worldZ 512

#define chunkSize 16

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

extern GRRLIB_texImg *tex_blockicons[256];

extern unsigned char theWorld[worldY][worldX][worldZ];
extern unsigned char lighting[worldX][worldZ];
