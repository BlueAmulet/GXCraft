#pragma once
#include <gccore.h>

typedef struct {
	u16 size;
	u16 index;
	s16 *vertex;
	u16 *color;
	f32 *texcoord;
} displayList;

displayList *displist_create(u16 size);
void displist_start();
void displist_clear(displayList *list);
void displist_bind(displayList *list);
void displist_unbind();
void displist_begin(u16 vtxcnt);
void displist_end();
void displist_add(s16 x, s16 y, s16 z, u16 c, f32 u, f32 v);
void displist_render(displayList *list);
