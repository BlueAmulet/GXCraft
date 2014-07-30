#include "display_list.h"

displayList *dlist;

displayList *displist_create(u16 size)
{
	displayList * list = malloc(sizeof(displayList));
	list->size = size;
	list->index = 0;
	list->vertex = malloc(sizeof(s16)*size*3);
	list->color = malloc(sizeof(u16)*size);
	list->texcoord = malloc(sizeof(u8)*size*2);
	return list;
}

void displist_start()
{
	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_INDEX16);
	GX_SetVtxDesc(GX_VA_CLR0, GX_INDEX16);
	GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX16);

	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA4, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);
	
	/*GX_SetTevColorIn(GX_TEVSTAGE0,GX_CC_RASC,GX_CC_ONE,GX_CC_TEXC,GX_CC_ZERO);
	GX_SetTevAlphaIn(GX_TEVSTAGE0,GX_CA_TEXA,GX_CA_RASA,GX_CA_TEXA,GX_CC_RASA);
	GX_SetTevColorOp(GX_TEVSTAGE0,GX_TEV_ADD,GX_TB_ZERO,GX_CS_SCALE_1,GX_TRUE,GX_TEVPREV);
	GX_SetTevAlphaOp(GX_TEVSTAGE0,GX_TEV_COMP_A8_GT,GX_TB_ZERO,GX_CS_SCALE_1,GX_TRUE,GX_TEVPREV);*/
}

void displist_clear(displayList *list)
{
	list->index = 0;
}

void displist_bind(displayList *list)
{
	dlist = list;
}

void displist_add(s16 x, s16 y, s16 z, u16 c, u8 u, u8 v)
{
	u16 idx = dlist->index;
	dlist->vertex[idx*3+0] = x;
	dlist->vertex[idx*3+1] = y;
	dlist->vertex[idx*3+2] = z;
	
	dlist->color[idx] = c;
	
	dlist->texcoord[idx*2+0] = u;
	dlist->texcoord[idx*2+1] = v;
	
	dlist->index++;
}

void displist_render(displayList *list)
{
	GX_SetArray (GX_VA_POS,  list->vertex, sizeof(s16) * 3);
	GX_SetArray (GX_VA_CLR0, list->color, sizeof(u16));
	GX_SetArray (GX_VA_TEX0, list->texcoord, sizeof(u8) * 2);
	
	u16 i;
	GX_Begin(GX_QUADS, GX_VTXFMT0, list->index);
		for(i = 0; i < list->index ; i++)
		{
			GX_Color1x16(i);
			GX_Position1x16(i);	
			GX_TexCoord1x16(i);	
		}	
	GX_End ();
	
	DCFlushRange(list->vertex, sizeof(s16) * 3 * list->size);
	DCFlushRange(list->color, sizeof(u16) * list->size);
	DCFlushRange(list->texcoord, sizeof(u8) * 2 * list->size);
}
