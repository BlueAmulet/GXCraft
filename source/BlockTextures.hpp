#pragma once

#include <gctypes.h>

typedef struct
{
	f32 u0,v0;
	f32 u1,v1;
} blockTexture;

void initTextures();
blockTexture *getTexture(int x, int y);
