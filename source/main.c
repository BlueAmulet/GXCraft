/*===========================================
    GXCraft by gamax92 and ds84182
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "textures/font.h"
#include "textures/inventory.h"
#include "textures/inv_select.h"
#include "textures/terrain_blocks.h"
#include "textures/cursor.h"

#include "main.h"
#include "player.h"
#include "controls.h"
#include "block.h"
#include "terrain.h"
#include "render.h"
#include "netcat_logger.h"
#include "chunked_render.h"
#include "block_textures.h"

#include "block/block_includes.h"

unsigned char theWorld[worldY][worldX][worldZ];
unsigned char lighting[worldX][worldZ];

player thePlayer;

inline double to_degrees(double radians) {
	return radians*(180.0f/M_PI);
}

inline double to_radians(double degrees) {
	return (degrees*M_PI)/180.0f;
}

// Safe block placement;
static void setBlock(int x, int y, int z, unsigned char blockID) {
	if (x >= 0 && x < worldX && y >= 0 && y < worldY && z >= 0 && z < worldZ) {
		theWorld[y][x][z] = blockID;
		if (blockID == 18)
			return;
		if (blockID != 0) {
			if (lighting[x][z] < y)
				lighting[x][z] = y;
		} else {
			if (y >= lighting[x][z]) {
				int sy;
				for (sy = y - 1; y >= 0; sy--) {
					if (theWorld[sy][x][z] != 0 && theWorld[sy][x][z] != 18) {
						lighting[x][z] = sy;
						break;
					}
				}
			}
		}
	}
}

// Safe block retrieval;
static unsigned char getBlock(int x, int y, int z) {
	if (x >= 0 && x < worldX && y >= 0 && y < worldY && z >= 0 && z < worldZ)
		return theWorld[y][x][z];
	return 0;
}

static void placeTree(int x, int y, int z) {
	if (getBlock(x,y-1,z) == 2)
		setBlock(x,y-1,z,3);
	int lx,ly,lz;
	for (ly = y + 3; ly <= y + 4; ly++) {
		for (lx = x - 2; lx <= x + 2; lx++) {
			for (lz = z - 2; lz <= z + 2; lz++) {
				setBlock(lx,ly,lz,18);
			}
		}
	}
	for (lx = x - 1; lx <= x + 1; lx++) {
		for (lz = z - 1; lz <= z + 1; lz++) {
			setBlock(lx,y+5,lz,18);
		}
	}
	setBlock(x,y+6,z,18);
	setBlock(x-1,y+6,z,18);
	setBlock(x+1,y+6,z,18);
	setBlock(x,y+6,z-1,18);
	setBlock(x,y+6,z+1,18);
	for (ly = y; ly < y + 6; ly++)
		setBlock(x,ly,z,17);
}

static void generateWorld() {
	generateTerrain();
	short x,y,z;
	for (x = 0; x < worldX; x++) {
		for (z = 0; z < worldZ; z++) {
			double terrainPiece = floor(((terrainData[x][z] - minY) / (maxY - minY) * (worldY - 2)) + 1);
			if (terrainPiece < 15)
				lighting[x][z] = 15;
			else
				lighting[x][z] = terrainPiece;
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
					int type = rand() % 1000;
					if (type == 1)
						placeTree(x,y,z);
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
	// TODO: Add Flower pockets
}

static void initializeBlocks();
static u8 CalculateFrameRate();

typedef enum {REGISTER, GENERATE, INGAME, INVENTORY, NUNCHUK} gamestate;

int main() {
	netcat_console();

	time_t t;
	srand((unsigned) time(&t));

	int renderDistance = 64;

	gamestate status = REGISTER;
	u8 FPS = 0;

	memset(theWorld, 255, sizeof theWorld);

	bool rerenderDisplayList = true;
	bool exitloop = false;
	int dluse = 0;
	int dlsize = 0;

	// Initialize the player
	thePlayer.posX = 331.5;
	thePlayer.posZ = 69.5;
	thePlayer.motionX = 0;
	thePlayer.motionY = 0;
	thePlayer.motionZ = 0;
	thePlayer.lookX = 0;
	thePlayer.lookY = 0;
	thePlayer.lookZ = 0;
	memset(thePlayer.inventory, 0, sizeof thePlayer.inventory);
	thePlayer.flying = true;
	thePlayer.timer = 0;

	int displistX = 256;
	int displistZ = 256;

	GRRLIB_Init();
	WPAD_Init();
	chunked_init();
	initTextures();

	WPADData *data;
	ir_t IR_0;

	GRRLIB_Settings.antialias = false;

	GRRLIB_texImg *tex_font       = GRRLIB_LoadTexture(font);
	GRRLIB_texImg *tex_inventory  = GRRLIB_LoadTexture(inventory);
	GRRLIB_texImg *tex_inv_select = GRRLIB_LoadTexture(inv_select);
	GRRLIB_texImg *tex_terrain    = GRRLIB_LoadTexture(terrain_blocks);
	GRRLIB_texImg *tex_cursor     = GRRLIB_LoadTexture(cursor);

	GRRLIB_SetMidHandle(tex_cursor, true);

	GRRLIB_InitTileSet(tex_font, 16, 16, 32);

	GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

	unsigned char inv_blocks[42] = {
	 1,  4, 45, 03, 05, 17, 18, 20, 43,
	48,  6, 37, 38, 39, 40, 12, 13, 19,
	21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 16, 15,
	14, 42, 41, 47, 46, 49 };

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
			GX_SetLineWidth(15, GX_TO_ONE);
			status = INGAME;
			break;
		case INVENTORY:
		case INGAME: // Main loop
			// Reset Motion
			thePlayer.motionX = 0;
			thePlayer.motionZ = 0;
			if (thePlayer.flying) {
				thePlayer.motionY = 0;
			} else {
				thePlayer.motionY = 0;
			}

			if (thePlayer.timer > 0)
				thePlayer.timer--;

			WPAD_ScanPads();
			WPAD_IR(WPAD_CHAN_0, &IR_0);

			data = WPAD_Data(WPAD_CHAN_0);
			if (data->exp.type != WPAD_EXP_NUNCHUK)
				status = NUNCHUK;

			if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_HOME) {
				exitloop = true;
				break;
			}
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
			if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_1) {
				thePlayer.flying = !thePlayer.flying;
			}
			if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_NUNCHUK_BUTTON_C) {
				status = status == INVENTORY ? INGAME : INVENTORY;
			}
			thePlayer.lookX += WPAD_StickX(WPAD_CHAN_0, 0) / 128.f;
			thePlayer.lookY -= WPAD_StickY(WPAD_CHAN_0, 0) / 128.f;

			if (thePlayer.flying) {
				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_PLUS)
					thePlayer.motionY += 0.1;
				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_MINUS)
					thePlayer.motionY -= 0.1;
			} else {
				if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_PLUS) {
					thePlayer.inventory[9] += 1;
					thePlayer.inventory[9] %= 9;
				}
				if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_MINUS) {
					if (thePlayer.inventory[9] == 0) thePlayer.inventory[9] = 9;
					thePlayer.inventory[9] -= 1;
				}
				//TODO: Check for Nunchuk Z (Jumping)
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
			GX_SetFog(GX_FOG_LIN, renderDistance/2 - 4, renderDistance - 8, 0.1, 1000, c);

			GRRLIB_ObjectViewBegin();
			GRRLIB_ObjectViewTrans(-thePlayer.posX, -thePlayer.posY - 1.625, -thePlayer.posZ);
			GRRLIB_ObjectViewRotate(0, thePlayer.lookX, 0);
			GRRLIB_ObjectViewRotate(thePlayer.lookY, 0, 0);
			GRRLIB_ObjectViewEnd();

			if (abs(displistX - thePlayer.posX) + abs(displistZ - thePlayer.posZ) > 8) {
				netcat_log("rerender display list because player too far from last render point\n");
				rerenderDisplayList = true;
			}

			double xLook =  sin(to_radians(thePlayer.lookX))*cos(to_radians(thePlayer.lookY));
			double yLook = -sin(to_radians(thePlayer.lookY));
			double zLook = -cos(to_radians(thePlayer.lookX))*cos(to_radians(thePlayer.lookY));

			float i;
			for (i = 0; i < 7; i += 0.01) { // TODO: This may be too precise?
				unsigned char block = getBlock(xLook*i+thePlayer.posX,yLook*i+thePlayer.posY+1.625,zLook*i+thePlayer.posZ);
				if (block != 0 && block != 8 && block != 10) {
					int selBlockX = (int)(xLook*i+thePlayer.posX);
					int selBlockY = (int)(yLook*i+thePlayer.posY+1.625);
					int selBlockZ = (int)(zLook*i+thePlayer.posZ);
					GRRLIB_SetTexture(tex_inventory, 0);
					drawSelectionBlock(selBlockX, selBlockY, selBlockZ);

					double blockSelOffX = fmod(xLook*i+thePlayer.posX,1)-0.5f;
					double blockSelOffY = fmod(yLook*i+thePlayer.posY+1.625,1)-0.5f;
					double blockSelOffZ = fmod(zLook*i+thePlayer.posZ,1)-0.5f;

					double aBlockSelOffX = fabs(blockSelOffX);
					double aBlockSelOffY = fabs(blockSelOffY);
					double aBlockSelOffZ = fabs(blockSelOffZ);

					signed char faceBlockX = 0;
					signed char faceBlockY = 0;
					signed char faceBlockZ = 0;
					if (aBlockSelOffX > aBlockSelOffY && aBlockSelOffX > aBlockSelOffZ)
						faceBlockX = aBlockSelOffX/blockSelOffX;
					if (aBlockSelOffY > aBlockSelOffX && aBlockSelOffY > aBlockSelOffZ)
						faceBlockY = aBlockSelOffY/blockSelOffY;
					if (aBlockSelOffZ > aBlockSelOffX && aBlockSelOffZ > aBlockSelOffY)
						faceBlockZ = aBlockSelOffZ/blockSelOffZ;

					if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_B && status != INVENTORY && thePlayer.timer == 0 && getBlock(selBlockX,selBlockY,selBlockZ) != 7) {
						setBlock(selBlockX,selBlockY,selBlockZ,0);
						chunked_rerenderChunk(floor(selBlockX/16), floor(selBlockZ/16), true);
						if (selBlockX % 16 == 15) chunked_rerenderChunk(floor(selBlockX/16)+1,floor(selBlockZ/16),true);
						if (selBlockX % 16 ==  0) chunked_rerenderChunk(floor(selBlockX/16)-1,floor(selBlockZ/16),true);
						if (selBlockZ % 16 == 15) chunked_rerenderChunk(floor(selBlockX/16),floor(selBlockZ/16)+1,true);
						if (selBlockZ % 16 ==  0) chunked_rerenderChunk(floor(selBlockX/16),floor(selBlockZ/16)-1,true);
						thePlayer.timer = 18;
					} else if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_A && status != INVENTORY && thePlayer.timer == 0) {
						selBlockX+=faceBlockX;
						selBlockY+=faceBlockY;
						selBlockZ+=faceBlockZ;

						setBlock(selBlockX,selBlockY,selBlockZ,1);
						chunked_rerenderChunk(floor(selBlockX/16), floor(selBlockZ/16), true);
						if (selBlockX % 16 == 15) chunked_rerenderChunk(floor(selBlockX/16)+1,floor(selBlockZ/16),true);
						if (selBlockX % 16 ==  0) chunked_rerenderChunk(floor(selBlockX/16)-1,floor(selBlockZ/16),true);
						if (selBlockZ % 16 == 15) chunked_rerenderChunk(floor(selBlockX/16),floor(selBlockZ/16)+1,true);
						if (selBlockZ % 16 ==  0) chunked_rerenderChunk(floor(selBlockX/16),floor(selBlockZ/16)-1,true);
						thePlayer.timer = 18;
					}
					break;
				}
			}

			if (rerenderDisplayList)
			{
				netcat_log("rerendering display list\n");
				rerenderDisplayList = false;
				displistX = thePlayer.posX;
				displistZ = thePlayer.posZ;
				chunked_refresh(renderDistance, thePlayer);
				dluse = chunked_getfifousage();
				dlsize = chunked_getfifototal();
			}
			GRRLIB_SetTexture(tex_terrain, 0);
			chunked_render(thePlayer);

			// Draw 2D elements
			//netcat_log("switching 2d\n");
			GRRLIB_2dMode();

			GRRLIB_SetBlend(GRRLIB_BLEND_INV);
			GRRLIB_Rectangle(308, 239, 24, 2, 0xFFFFFFFF, true);
			GRRLIB_Rectangle(319, 228, 2, 11, 0xFFFFFFFF, true);
			GRRLIB_Rectangle(319, 241, 2, 11, 0xFFFFFFFF, true);
			GRRLIB_SetBlend(GRRLIB_BLEND_ALPHA);

			GRRLIB_DrawImg(138, 436, tex_inventory, 0, 2, 2, 0xFFFFFFFF);

			// TODO: Draw Actual blocks
			int b;
			for (b = 0; b < 9; b++) {
				GXCraft_DrawText(b * 40 + 144, 440, tex_font, "%02d", thePlayer.inventory[b]);
			}

			GRRLIB_DrawImg(thePlayer.inventory[9] * 40 + 136, 434, tex_inv_select, 0, 2, 2, 0xFFFFFFFF);

			if (status == INVENTORY) {
				GRRLIB_Rectangle(80, 60, 480, 300, 0x0000007F, true);

				GXCraft_DrawText(224, 80, tex_font, "SELECT BLOCK");

				// TODO: Draw selection box (52x52)

				// TODO: Draw blocks on this.
				int x, y;
				for (b = 0; b < 42; b++) {
					x = b % 9;
					y = floor(b/9);
					GXCraft_DrawText(x * 48 + 110, y * 48 + 114, tex_font, "%02d", inv_blocks[b]);
				}
				GRRLIB_DrawImg(IR_0.sx, IR_0.sy, tex_cursor, IR_0.angle, 1, 1, 0xFFFFFFFF);
			}

			// Draw debugging elements
			GXCraft_DrawText(10,  25, tex_font, "FPS: %d", FPS);
			GXCraft_DrawText(10,  40, tex_font, "PX:% 7.2f", thePlayer.posX);
			GXCraft_DrawText(10,  55, tex_font, "PY:% 7.2f", thePlayer.posY);
			GXCraft_DrawText(10,  70, tex_font, "PZ:% 7.2f", thePlayer.posZ);
			GXCraft_DrawText(10,  85, tex_font, "LX:% 7.2f", thePlayer.lookX);
			GXCraft_DrawText(10, 100, tex_font, "LY:% 7.2f", thePlayer.lookY);
			GXCraft_DrawText(10, 115, tex_font, "LZ:% 7.2f", thePlayer.lookZ);
			GXCraft_DrawText(10, 130, tex_font, "DLSIZE: %i/%i (%i%%)", dluse, dlsize, dluse*100/dlsize);

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

			GRRLIB_SetTexture(tex_terrain, 0);
			chunked_render(thePlayer);

			//Complain to user
			GRRLIB_2dMode();
			GRRLIB_Rectangle(0, 0, 640, 480, 0x0000007F, true);
			GXCraft_DrawText(144, 232, tex_font, "PLEASE CONNECT NUNCHUK");
			GRRLIB_Render();
			break;
		}
	}
	netcat_log("ending...\n");
	netcat_close();

	GRRLIB_FreeTexture(tex_font);
	GRRLIB_FreeTexture(tex_inventory);
	GRRLIB_FreeTexture(tex_inv_select);
	GRRLIB_FreeTexture(tex_terrain);
	GRRLIB_FreeTexture(tex_cursor);

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
	obsidian_init();
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
