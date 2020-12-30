#include <cmath>
#include <malloc.h>

#include <gccore.h>

#include "DisplayList.hpp"

DisplayList::DisplayList(u16 size) {
	this->size = size;
	this->index = 0;
	this->vertex = static_cast<s16*>(malloc(sizeof(s16) * size * 3));
	this->color = static_cast<u16*>(malloc(sizeof(u16) * size));
	this->texcoord = static_cast<u8*>(malloc(sizeof(u8) * size * 2));
}

void DisplayList::start() {
	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_INDEX16);
	GX_SetVtxDesc(GX_VA_CLR0, GX_INDEX16);
	GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX16);

	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA4, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 8);
}

void DisplayList::clear() {
	this->index = 0;
}

void DisplayList::add(s16 x, s16 y, s16 z, u16 c, f32 u, f32 v) {
	u16 idx = this->index;
	this->vertex[idx * 3 + 0] = x;
	this->vertex[idx * 3 + 1] = y;
	this->vertex[idx * 3 + 2] = z;

	this->color[idx] = c;

	this->texcoord[idx * 2 + 0] = static_cast<u8>(u * 256);
	this->texcoord[idx * 2 + 1] = static_cast<u8>(v * 256);

	this->index++;

	if (this->index >= this->size) {
		u16 size = this->size * 2;
		this->size = size;
		this->vertex = static_cast<s16*>(realloc(this->vertex, sizeof(s16) * size * 3));
		this->color = static_cast<u16*>(realloc(this->color, sizeof(u16) * size));
		this->texcoord = static_cast<u8*>(realloc(this->texcoord, sizeof(u8) * size * 2));
	}
}

void DisplayList::fit() {
	u16 size = pow(2.0, fmax(ceil(log2(this->index)), 5.0));
	if (size <= this->size) {
		this->size = size;
		this->vertex = static_cast<s16*>(realloc(this->vertex, sizeof(s16) * size * 3));
		this->color = static_cast<u16*>(realloc(this->color, sizeof(u16) * size));
		this->texcoord = static_cast<u8*>(realloc(this->texcoord, sizeof(u8) * size * 2));
	}
}

void DisplayList::render() {
	if (!this->index) {
		return;
	}

	// TODO: Flush range if list data is dirty
	DCFlushRange(this->vertex, sizeof(s16) * 3 * this->size);
	DCFlushRange(this->color, sizeof(u16) * this->size);
	DCFlushRange(this->texcoord, sizeof(u8) * 2 * this->size);

	GX_SetArray(GX_VA_POS, this->vertex, sizeof(s16) * 3);
	GX_SetArray(GX_VA_CLR0, this->color, sizeof(u16));
	GX_SetArray(GX_VA_TEX0, this->texcoord, sizeof(u8) * 2);

	GX_Begin(GX_QUADS, GX_VTXFMT0, this->index);
	for (u16 i = 0; i < this->index; i++) {
		GX_Color1x16(i);
		GX_Position1x16(i);
		GX_TexCoord1x16(i);
	}
	GX_End();
}
