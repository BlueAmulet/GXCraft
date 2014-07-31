#pragma once
#include "display_list.h"
#include "player.h"
#define nRenderChunks 256

typedef struct
{
	bool active;
	unsigned short x, z;
	displayList *list;
	displayList *blendlist;
} renderchunk;

extern renderchunk *renderchunks[nRenderChunks];
void chunked_init();
void chunked_rerenderChunk(signed short cx, signed short cz, bool force);
void chunked_refresh(int renderDistance, player thePlayer);
void chunked_render(player thePlayer);
int chunked_getfifousage();
int chunked_getfifototal();
