#pragma once
#include "block_textures.h"

void GXCraft_DrawText(f32 xPos, f32 yPos, GRRLIB_texImg* font, const char* format, ...);
bool testFace(unsigned char face);
void drawBlock(int xPos, int yPos, int zPos, blockTexture *tex);
void drawBlockCrossed(int xPos, int yPos, int zPos, blockTexture *tex);
void drawMultiTexBlock(int xPos, int yPos, int zPos, blockTexture *texTop, blockTexture *texSide, blockTexture *texBott);
void drawSelectionBlock(int xPos, int yPos, int zPos);
