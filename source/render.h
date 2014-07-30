#pragma once
#include "block_textures.h"

bool testFace(unsigned char face);
void drawBlock(int xPos, int yPos, int zPos, blockTexture *tex);
void drawBlockCrossed(int xPos, int yPos, int zPos, blockTexture *tex);
void drawMultiTexBlock(int xPos, int yPos, int zPos, blockTexture *texTop, blockTexture *texSide, blockTexture *texBott);
void drawSelectionBlock(int xPos, int yPos, int zPos);
