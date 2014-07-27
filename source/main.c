/*===========================================
    GXCraft by gamax92 and ds84182
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "textures/font.h"

#include "main.h"
#include "player.h"
#include "controls.h"
#include "render.h"
#include "block.h"
#include "terrain.h"
#include "netcat_logger.h"

#include "block/block_includes.h"

#define chunkX 16
#define chunkZ 16

#define displayListSize ((8*1024*1024)+63)

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

unsigned char theWorld[worldY][worldX][worldZ];

player thePlayer;

static vu8	readyForCopy;

inline double to_degrees(double radians) {
    return radians*(180.0f/M_PI);
}

inline double to_radians(double degrees) {
    return (degrees*M_PI)/180.0f;
}

static void generateWorld() {
	generateTerrain();
	short x,y,z;
	for (x = 0; x < worldX; x++) {
		for (z = 0; z < worldZ; z++) {
			//double terrainPiece = floor(((terrainData[x][z] - minY) / (maxY - minY) * 16) + 24);
			double terrainPiece = floor(((terrainData[x][z] - minY) / (maxY - minY) * (worldY - 2)) + 1);
			for (y = 0; y < worldY; y++) {
				if (y == 0)
					theWorld[y][x][z] = 7;
				else if (y < terrainPiece - 2)
					theWorld[y][x][z] = 1;
				else if (y < terrainPiece && y >= terrainPiece - 2)
					theWorld[y][x][z] = 3;
				else if (y == terrainPiece) {
					if (terrainPiece < 15)
						theWorld[y][x][z] = 3;
					else
						theWorld[y][x][z] = 2;
				} else if (y == terrainPiece + 1 && terrainPiece >= 16) {
					int type = rand() % 100;
					if (type == 0)
						theWorld[y][x][z] = 37;
					else if (type == 1)
						theWorld[y][x][z] = 38;
					else
						theWorld[y][x][z] = 0;
				} else if (theWorld[y][x][z] == 255) {	
					if (y < 16)
						theWorld[y][x][z] = 8;
					else
						theWorld[y][x][z] = 0;
				}
			}
		}
	}
}

static void initializeBlocks();
static void cleanBlocks();
static u8 CalculateFrameRate();

typedef enum {REGISTER, GENERATE, INGAME, NUNCHUK} gamestate;

int main() {
	netcat_console();

	time_t t;
	srand((unsigned) time(&t));

	u32 TEXT_COLOR = 0x000000FF;

	int renderDistance = 64;

	gamestate status = REGISTER;
	u8 FPS = 0;

	memset(theWorld, 255, sizeof theWorld);
	
	void *displayList = memalign(32, displayListSize);
	memset(displayList, 0, displayListSize);
	bool rerenderDisplayList = true;
	bool exitloop = false;
	int dlrendersize = 0;

	thePlayer.posX = 331.5;
	thePlayer.posZ = 69.5;
	thePlayer.motionX = 0;
	thePlayer.motionY = 0;
	thePlayer.motionZ = 0;
	thePlayer.lookX = 0;
	thePlayer.lookY = 0;
	thePlayer.lookZ = 0;
	thePlayer.flying = true;

	int displistX = 256;
	int displistZ = 256;

    GRRLIB_Init();
    WPAD_Init();
	WPADData *data;

	GRRLIB_Settings.antialias = false;

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    while (!exitloop) {
		switch(status) {
		case REGISTER: // Register blocks
		    GRRLIB_2dMode();
			netcat_log("registering blocks\n");
			GRRLIB_Printf(152, 232, tex_font, 0xFFFFFFFF, 1, "REGISTERING BLOCKS...");
		    GRRLIB_Render();
			initializeBlocks();
			status = GENERATE;
			break;
		case GENERATE: // Generate the world
		    GRRLIB_2dMode();
			netcat_log("generating world\n");
			GRRLIB_Printf(160, 232, tex_font, 0xFFFFFFFF, 1, "GENERATING WORLD ...");
		    GRRLIB_Render();
			generateWorld();
			int y;
			for (y = worldY - 1; y >= 0; y--) {
				if (theWorld[y][(int)thePlayer.posX][(int)thePlayer.posZ] != 0) {
					thePlayer.posY = y + 1;
					break;
				}
			}
			GRRLIB_SetBackgroundColour(0x9E, 0xCE, 0xFF, 0xFF);
			status = INGAME;
			break;
		case INGAME: // Main loop
			if (thePlayer.flying) { // Reset Motion
				thePlayer.motionX = 0;
				thePlayer.motionY = 0;
				thePlayer.motionZ = 0;
			}
			GRRLIB_2dMode();
		    WPAD_ScanPads();

			data = WPAD_Data(WPAD_CHAN_0);
			if (data->exp.type != WPAD_EXP_NUNCHUK)
				status = NUNCHUK;

		    if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_HOME) exit(0);
			if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_UP) {
				thePlayer.motionX += sin(to_radians(thePlayer.lookX)) * 0.1;
				thePlayer.motionZ -= cos(to_radians(thePlayer.lookX)) * 0.1;
			}
			if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_DOWN) {
				thePlayer.motionX -= sin(to_radians(thePlayer.lookX)) * 0.1;
				thePlayer.motionZ += cos(to_radians(thePlayer.lookX)) * 0.1;
			}
			if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_LEFT) {
				thePlayer.motionX -= cos(to_radians(thePlayer.lookX)) * 0.1;
				thePlayer.motionZ -= sin(to_radians(thePlayer.lookX)) * 0.1;
			}
			if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_RIGHT) {
				thePlayer.motionX += cos(to_radians(thePlayer.lookX)) * 0.1;
				thePlayer.motionZ += sin(to_radians(thePlayer.lookX)) * 0.1;
			}
			thePlayer.lookX += WPAD_StickX(WPAD_CHAN_0, 0) / 128.f;
			thePlayer.lookY -= WPAD_StickY(WPAD_CHAN_0, 0) / 128.f;

			if (thePlayer.flying) {
				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_PLUS)
					thePlayer.motionY += 0.1;
				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_MINUS)
					thePlayer.motionY -= 0.1;
			} else {
				/*
				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_PLUS)
				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_MINUS)
				TODO: Check for Nunchuk Z (Jumping)
				*/
			}

			// Apply motion to player
			thePlayer.posX += thePlayer.motionX;
			thePlayer.posY += thePlayer.motionY;
			thePlayer.posZ += thePlayer.motionZ;

			// Keep values in bound
			if (thePlayer.lookX > 180)       thePlayer.lookX -= 360;
			else if (thePlayer.lookX < -180) thePlayer.lookX += 360;
			if (thePlayer.lookY > 90)        thePlayer.lookY =  90;
			else if (thePlayer.lookY < -90)  thePlayer.lookY = -90;
			if (thePlayer.lookZ > 180)       thePlayer.lookZ -= 360;
			else if (thePlayer.lookZ < -180) thePlayer.lookZ += 360;
			
			//netcat_log("switching 3d\n");
		    GRRLIB_3dMode(0.1, 1000, 45, 1, 0);

			GXColor c = {0x9E, 0xCE, 0xFF};
			GX_SetFog(GX_FOG_LIN, renderDistance/2, renderDistance, 0.1, 1000, c);

			GRRLIB_ObjectViewBegin();
			GRRLIB_ObjectViewTrans(-thePlayer.posX, -thePlayer.posY - 1.625, -thePlayer.posZ);
			GRRLIB_ObjectViewRotate(0, thePlayer.lookX, 0);
			GRRLIB_ObjectViewRotate(thePlayer.lookY, 0, 0);
			GRRLIB_ObjectViewEnd();

			if (abs(displistX - thePlayer.posX) + abs(displistZ - thePlayer.posZ) > 8) {
				netcat_log("rerender display list because player too far from last render point\n");
				rerenderDisplayList = true;
			}
			
			//GRRLIB clears the vertex formats on mode switch
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA4, 0);
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);

			if (rerenderDisplayList)
			{
				netcat_log("rerendering display list\n");
				rerenderDisplayList = false;
				displistX = thePlayer.posX;
				displistZ = thePlayer.posZ;
				lastTex = NULL;
				GX_BeginDispList(displayList, displayListSize);
				int x;
				int y;
				int z;
				for (y = worldY - 1; y >= 0; y--) {
					for (x = max(thePlayer.posX - renderDistance,0); x <= min(worldX-1,thePlayer.posX + renderDistance); x++) {
						for (z = max(thePlayer.posZ - renderDistance,0); z <= min(worldZ-1,thePlayer.posZ + renderDistance); z++) {
							unsigned char blockID = theWorld[y][x][z];
							if (blockID != 0) {
								blockEntry entry = blockRegistry[blockID];
								entry.renderBlock(x, y, z, 0);
							}
						}
					}
				}
				netcat_log("rendered pass 0\n");
				
				GX_SetTevColorIn(GX_TEVSTAGE0,GX_CC_RASC,GX_CC_ONE,GX_CC_TEXC,GX_CC_ZERO);
				GX_SetTevAlphaIn(GX_TEVSTAGE0,GX_CA_TEXA,GX_CA_RASA,GX_CA_TEXA,GX_CC_RASA);
				GX_SetTevColorOp(GX_TEVSTAGE0,GX_TEV_ADD,GX_TB_ZERO,GX_CS_SCALE_1,GX_TRUE,GX_TEVPREV);
				GX_SetTevAlphaOp(GX_TEVSTAGE0,GX_TEV_COMP_A8_GT,GX_TB_ZERO,GX_CS_SCALE_1,GX_TRUE,GX_TEVPREV);
				
				for (y = 0; y < worldY; y++) {
					for (x = max(thePlayer.posX - renderDistance,0); x <= min(worldX-1,thePlayer.posX + renderDistance); x++) {
						for (z = max(thePlayer.posZ - renderDistance,0); z <= min(worldZ-1,thePlayer.posZ + renderDistance); z++) {
							unsigned char blockID = theWorld[y][x][z];
							if (blockID != 0) {
								blockEntry entry = blockRegistry[blockID];
								entry.renderBlock(x, y, z, 1);
							}
						}
					}
				}
				netcat_log("rendered pass 1\n");
				dlrendersize = GX_EndDispList();
				netcat_log("invalidating range\n");
				DCFlushRange(displayList,displayListSize); //so real wii doesn't crash and shit... i think
				netcat_log("invalidated range\n");
			}
			GX_CallDispList(displayList, dlrendersize);

		    // Draw 2D elements
			//netcat_log("switching 2d\n");
		    GRRLIB_2dMode();
			GRRLIB_Printf(10,  25, tex_font, TEXT_COLOR, 1, "FPS: %d", FPS);
			GRRLIB_Printf(10,  40, tex_font, TEXT_COLOR, 1, "PX:% 7.2f", thePlayer.posX);
			GRRLIB_Printf(10,  55, tex_font, TEXT_COLOR, 1, "PY:% 7.2f", thePlayer.posY);
			GRRLIB_Printf(10,  70, tex_font, TEXT_COLOR, 1, "PZ:% 7.2f", thePlayer.posZ);
			GRRLIB_Printf(10,  85, tex_font, TEXT_COLOR, 1, "LX:% 7.2f", thePlayer.lookX);
			GRRLIB_Printf(10, 100, tex_font, TEXT_COLOR, 1, "LY:% 7.2f", thePlayer.lookY);
			GRRLIB_Printf(10, 115, tex_font, TEXT_COLOR, 1, "LZ:% 7.2f", thePlayer.lookZ);
			GRRLIB_Printf(10, 130, tex_font, TEXT_COLOR, 1, "DLSIZE: %i/%i (%i%%)", dlrendersize, displayListSize, dlrendersize*100/displayListSize);
		    GRRLIB_Render();
			FPS = CalculateFrameRate();
			break;
		case NUNCHUK:
			GRRLIB_2dMode();
		    WPAD_ScanPads();

			data = WPAD_Data(WPAD_CHAN_0);
			if (data->exp.type == WPAD_EXP_NUNCHUK)
				status = INGAME;

		    GRRLIB_3dMode(0.1, 1000, 45, 1, 0);

			GX_SetFog(GX_FOG_LIN, renderDistance/2, renderDistance, 0.1, 1000, c);

			GRRLIB_ObjectViewBegin();
			GRRLIB_ObjectViewTrans(-thePlayer.posX, -thePlayer.posY - 1.625, -thePlayer.posZ);
			GRRLIB_ObjectViewRotate(0, thePlayer.lookX, 0);
			GRRLIB_ObjectViewRotate(thePlayer.lookY, 0, 0);
			GRRLIB_ObjectViewEnd();
			
			//GRRLIB clears the vertex formats on mode switch
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA4, 0);
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);

			GX_CallDispList(displayList, dlrendersize);

		    // Complain to user
		    GRRLIB_2dMode();
			GRRLIB_Rectangle(0, 0, 640, 480, 0x0000007F, true);
			GRRLIB_Printf(144, 232, tex_font, TEXT_COLOR, 1, "PLEASE CONNECT NUNCHUK");
		    GRRLIB_Render();
			break;
		}
    }
	netcat_log("ending...\n");
	netcat_close();
    GRRLIB_FreeTexture(tex_font);

	// Ask blocks to cleanup
	cleanBlocks();

    GRRLIB_Exit();
    exit(0);
}

static void initializeBlocks() {
	bedrock_init();
	brick_init();
	cloth_aqua_init();
	cloth_black_init();
	cloth_blue_init();
	cloth_cyan_init();
	cloth_gray_init();
	cloth_green_init();
	cloth_indigo_init();
	cloth_lime_init();
	cloth_magenta_init();
	cloth_orange_init();
	cloth_pink_init();
	cloth_purple_init();
	cloth_red_init();
	cloth_violet_init();
	cloth_white_init();
	cloth_yellow_init();
	cobble_init();
	dirt_init();
	flower_init();
	glass_init();
	gold_init();
	grass_init();
	gravel_init();
	iron_init();
	lava_init();
	leaves_init();
	log_init();
	mossy_init();
	mushroom_init();
	obsidna_init();
	ore_coal_init();
	ore_gold_init();
	ore_iron_init();
	redshroom_init();
	rose_init();
	sand_init();
	sapling_init();
	shelf_init();
	slab_init();
	sponge_init();
	stone_init();
	tnt_init();
	water_init();
	wood_init();
}

static void cleanBlocks() {
	bedrock_clean();
	brick_clean();
	cloth_aqua_clean();
	cloth_black_clean();
	cloth_blue_clean();
	cloth_cyan_clean();
	cloth_gray_clean();
	cloth_green_clean();
	cloth_indigo_clean();
	cloth_lime_clean();
	cloth_magenta_clean();
	cloth_orange_clean();
	cloth_pink_clean();
	cloth_purple_clean();
	cloth_red_clean();
	cloth_violet_clean();
	cloth_white_clean();
	cloth_yellow_clean();
	cobble_clean();
	dirt_clean();
	flower_clean();
	glass_clean();
	gold_clean();
	grass_clean();
	gravel_clean();
	iron_clean();
	lava_clean();
	leaves_clean();
	log_clean();
	mossy_clean();
	mushroom_clean();
	obsidna_clean();
	ore_coal_clean();
	ore_gold_clean();
	ore_iron_clean();
	redshroom_clean();
	rose_clean();
	sand_clean();
	sapling_clean();
	shelf_clean();
	slab_clean();
	sponge_clean();
	stone_clean();
	tnt_clean();
	water_clean();
	wood_clean();
}

static u8 CalculateFrameRate() {
    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}
