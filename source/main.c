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
#include "textures/clouds.h"

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
#include "textures/inv_blocks/block_icons.h"
#include "aabb.h"

#define ticks_to_secsf(ticks) (((f64)(ticks)/(f64)(TB_TIMER_CLOCK*1000)))

unsigned char theWorld[worldY][worldX][worldZ];
unsigned char lighting[worldX][worldZ];
GRRLIB_texImg *tex_blockicons[256];

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
		if (blockID == 6 || blockID == 18 || blockID == 20 || blockID == 37 || blockID == 38 || blockID == 39 || blockID == 40)
			return;
		if (blockID != 0) {
			if (lighting[x][z] < y)
				lighting[x][z] = y;
		} else {
			if (y >= lighting[x][z]) {
				int sy;
				for (sy = y - 1; y >= 0; sy--) {
					if (theWorld[sy][x][z] != 0 && theWorld[sy][x][z] != 6 && theWorld[sy][x][z] != 18 && theWorld[sy][x][z] != 20 && theWorld[sy][x][z] != 37 && theWorld[sy][x][z] != 38 && theWorld[sy][x][z] != 39 && theWorld[sy][x][z] != 40) {
						lighting[x][z] = sy;
						break;
					}
				}
			}
		}
	}
}

// Safe block retrieval;
unsigned char getBlock(int x, int y, int z) {
	if (x >= 0 && x < worldX && y >= 0 && y < worldY && z >= 0 && z < worldZ)
		return theWorld[y][x][z];
	return 0;
}

static void updateNeighbors(int x, int z) {
	if (x % 16 == 15) chunked_rerenderChunk(floor(x/16)+1,floor(z/16),true);
	if (x % 16 ==  0) chunked_rerenderChunk(floor(x/16)-1,floor(z/16),true);
	if (z % 16 == 15) chunked_rerenderChunk(floor(x/16),floor(z/16)+1,true);
	if (z % 16 ==  0) chunked_rerenderChunk(floor(x/16),floor(z/16)-1,true);
}

static void setIfAir(int x, int y, int z, unsigned char blockID) {
	unsigned char tBlockID = getBlock(x,y,z);
	if (tBlockID == 0 || tBlockID == 255)
		setBlock(x,y,z,blockID);
}

static void placeTree(int x, int y, int z) {
	if (getBlock(x,y-1,z) == 2)
		setBlock(x,y-1,z,3);
	int lx,ly,lz;
	for (ly = y + 3; ly <= y + 4; ly++) {
		for (lx = x - 2; lx <= x + 2; lx++) {
			for (lz = z - 2; lz <= z + 2; lz++) {
				setIfAir(lx,ly,lz,18);
			}
		}
	}
	for (lx = x - 1; lx <= x + 1; lx++) {
		for (lz = z - 1; lz <= z + 1; lz++) {
			setIfAir(lx,y+5,lz,18);
		}
	}
	setIfAir(x,y+6,z,18);
	setIfAir(x-1,y+6,z,18);
	setIfAir(x+1,y+6,z,18);
	setIfAir(x,y+6,z-1,18);
	setIfAir(x,y+6,z+1,18);
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
static void color85(char *buf, u32 color);
static int randnum(int x, int y);

typedef enum {NETCAT, REGISTER, GENERATE, INGAME, NUNCHUK, SCREENSHOT} gamestate;

int main() {
	//netcat_console(); // Comment this to disable netcat logger

	time_t t;
	srand((unsigned) time(&t));

	int renderDistance = 96;

	gamestate status;
	if (netcat_init)
		status = NETCAT;
	else
		status = REGISTER;

	u8 FPS = 0;

	memset(theWorld, 255, sizeof theWorld);

	bool rerenderDisplayList = true;
	bool exitloop = false;
	short scr_scanY = 0;	
	int dluse = 0;
	int dlsize = 0;

	// Initialize the player
	thePlayer.posX = 315.5;
	thePlayer.posZ = 64.5;
	thePlayer.motionX = 0;
	thePlayer.motionY = 0;
	thePlayer.motionZ = 0;
	thePlayer.lookX = 0;
	thePlayer.lookY = 0;
	thePlayer.lookZ = 0;
	unsigned char startinv[10] = {01,04,45,03,05,17,18,20,43,0};
	memcpy(thePlayer.inventory, startinv, sizeof(thePlayer.inventory));
	thePlayer.flying = true;
	thePlayer.select = false;
	thePlayer.timer = 0;

	bool wasUnder = false;

	int displistX = 256;
	int displistZ = 256;

	GRRLIB_Init();
	WPAD_Init();
	chunked_init();
	initTextures();

	WPADData *data;
	ir_t IR_0;

	GRRLIB_SetAntiAliasing(false);

	GRRLIB_texImg *tex_font       = GRRLIB_LoadTexture(font);
	GRRLIB_texImg *tex_inventory  = GRRLIB_LoadTexture(inventory);
	GRRLIB_texImg *tex_inv_select = GRRLIB_LoadTexture(inv_select);
	GRRLIB_texImg *tex_terrain    = GRRLIB_LoadTexture(terrain_blocks);
	GRRLIB_texImg *tex_cursor     = GRRLIB_LoadTexture(cursor);
	GRRLIB_texImg *tex_clouds     = GRRLIB_LoadTexture(clouds);
	GRRLIB_texImg *tex_tmpscreen  = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);

	//GRRLIB_texImg *tex_blockicons[256];

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

	f64 lastTime = ticks_to_secsf(gettime());

	double cloudPos = 0;

	struct mallinfo meminfo;

	while (!exitloop) {
		f64 thisTime = ticks_to_secsf(gettime());
		f64 deltaTime = (thisTime - lastTime);
		switch(status) {
		case NETCAT: // Listen for clients
			GRRLIB_2dMode();
			GRRLIB_Printf(152, 232, tex_font, 0xFFFFFFFF, 1, "WAITING FOR CLIENT...");
			GRRLIB_Render();
			netcat_accept();	
			status = REGISTER;
			break;
		case REGISTER: // Register blocks
			// TODO: Why does this not display?
			GRRLIB_2dMode();
			GRRLIB_Printf(152, 232, tex_font, 0xFFFFFFFF, 1, "REGISTERING BLOCKS...");
			GRRLIB_Render();
			netcat_log("registering blocks\n");
			initializeBlocks();
			load_bi();
			status = GENERATE;
			break;
		case GENERATE: // Generate the world
			GRRLIB_2dMode();
			GRRLIB_Printf(160, 232, tex_font, 0xFFFFFFFF, 1, "GENERATING WORLD ...");
			GRRLIB_Render();
			netcat_log("generating world\n");
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
				thePlayer.motionX += sin(to_radians(thePlayer.lookX)) * 6;
				thePlayer.motionZ -= cos(to_radians(thePlayer.lookX)) * 6;
			}
			if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_DOWN) {
				thePlayer.motionX -= sin(to_radians(thePlayer.lookX)) * 6;
				thePlayer.motionZ += cos(to_radians(thePlayer.lookX)) * 6;
			}
			if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_LEFT) {
				thePlayer.motionX -= cos(to_radians(thePlayer.lookX)) * 6;
				thePlayer.motionZ -= sin(to_radians(thePlayer.lookX)) * 6;
			}
			if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_RIGHT) {
				thePlayer.motionX += cos(to_radians(thePlayer.lookX)) * 6;
				thePlayer.motionZ += sin(to_radians(thePlayer.lookX)) * 6;
			}
			if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_1) {
				thePlayer.flying = !thePlayer.flying;
			}
			if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_NUNCHUK_BUTTON_C) {
				thePlayer.select = !thePlayer.select;
			}
			thePlayer.lookX += WPAD_StickX(WPAD_CHAN_0, 0) * deltaTime / 2.f;
			thePlayer.lookY -= WPAD_StickY(WPAD_CHAN_0, 0) * deltaTime / 2.f;

			if (thePlayer.flying) {
				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_PLUS)
					thePlayer.motionY += 6;
				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_MINUS)
					thePlayer.motionY -= 6;
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

			// Limit motion speed
			if (wasUnder) {
				thePlayer.motionX = thePlayer.motionX / 2;
				thePlayer.motionZ = thePlayer.motionZ / 2;
				// Restrict Y
			} else {
				// Restrict Y
			}
			
			double nextX = thePlayer.posX + thePlayer.motionX * deltaTime;
			double nextY = thePlayer.posY + thePlayer.motionY * deltaTime;
			double nextZ = thePlayer.posZ + thePlayer.motionZ * deltaTime;
			
			aabb player = aabb_alloc(nextX-0.25,nextY,nextZ-0.25,nextX+0.25,nextY+1.625,nextZ+0.25);
			while (aabb_resolvecollisionwithworld(player));
			//get values from player
			vector3 position = aabb_getMin(player);
			netcat_logf("position: %f, %f, %f\n",position.x,position.y,position.z);

			// Apply aabb to player
			thePlayer.posX = position.x+0.25;
			thePlayer.posY = position.y;
			thePlayer.posZ = position.z+0.25;
			aabb_dealloc(player);

			// Keep values in bound
			if (thePlayer.lookX > 180)       thePlayer.lookX -= 360;
			else if (thePlayer.lookX < -180) thePlayer.lookX += 360;
			if (thePlayer.lookY > 90)        thePlayer.lookY =  90;
			else if (thePlayer.lookY < -90)  thePlayer.lookY = -90;
			if (thePlayer.lookZ > 180)       thePlayer.lookZ -= 360;
			else if (thePlayer.lookZ < -180) thePlayer.lookZ += 360;

			//netcat_log("switching 3d\n");
			GRRLIB_3dMode(0.1, 1000, 45, 1, 0);

			if (getBlock(floor(thePlayer.posX),floor(thePlayer.posY+1.625),floor(thePlayer.posZ)) == 8) {
				if (!wasUnder) {
					GRRLIB_SetBackgroundColour(0x05, 0x05, 0x33, 0xFF);
					wasUnder = true;
				}
				GXColor c = {0x05, 0x05, 0x33};
				GX_SetFog(GX_FOG_LIN, 0, 32, 0.1, 1000, c);
			} else {
				if (wasUnder) {
					GRRLIB_SetBackgroundColour(0x9E, 0xCE, 0xFF, 0xFF);
					wasUnder = false;
				}
				GXColor c = {0x9E, 0xCE, 0xFF};
				GX_SetFog(GX_FOG_LIN, renderDistance - 32, renderDistance - chunkSize, 0.1, 1000, c);
			}

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
				unsigned char block = getBlock(floor(xLook*i+thePlayer.posX),floor(yLook*i+thePlayer.posY+1.625),floor(zLook*i+thePlayer.posZ));
				if (block != 0 && block != 8 && block != 10) {
					int selBlockX = floor(xLook*i+thePlayer.posX);
					int selBlockY = floor(yLook*i+thePlayer.posY+1.625);
					int selBlockZ = floor(zLook*i+thePlayer.posZ);
					GRRLIB_SetTexture(tex_inventory, false);
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

					if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_B && !thePlayer.select && thePlayer.timer == 0 && getBlock(selBlockX,selBlockY,selBlockZ) != 7) {
						setBlock(selBlockX,selBlockY,selBlockZ,0);
						chunked_rerenderChunk(floor(selBlockX/16), floor(selBlockZ/16), true);
						updateNeighbors(selBlockX, selBlockZ);
						thePlayer.timer = 18;
					} else if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_A && !thePlayer.select && thePlayer.timer == 0 && thePlayer.inventory[thePlayer.inventory[9]] != 0) {
						selBlockX+=faceBlockX;
						selBlockY+=faceBlockY;
						selBlockZ+=faceBlockZ;

						setBlock(selBlockX,selBlockY,selBlockZ,thePlayer.inventory[thePlayer.inventory[9]]);
						chunked_rerenderChunk(floor(selBlockX/16), floor(selBlockZ/16), true);
						updateNeighbors(selBlockX, selBlockZ);
						thePlayer.timer = 18;
					}
					break;
				}
			}
			
			// Random Block Update!
			for (i = 0; i < pow(renderDistance/16,2)*48; i++) {
				signed int rx = randnum(thePlayer.posX - renderDistance, thePlayer.posX + renderDistance);
				signed int ry = randnum(0, worldY - 1);
				signed int rz = randnum(thePlayer.posZ - renderDistance, thePlayer.posZ + renderDistance);
				unsigned char blockID = getBlock(rx,ry,rz);
				if (blockID == 3 && lighting[rx][rz] <= ry) {
					setBlock(rx,ry,rz,2);
					chunked_rerenderChunk(floor(rx/16), floor(rz/16), true);
					updateNeighbors(rx, rz);
				} else if (blockID == 2 && lighting[rx][rz] > ry) {
					setBlock(rx,ry,rz,3);
					chunked_rerenderChunk(floor(rx/16), floor(rz/16), true);
					updateNeighbors(rx, rz);
				} else if (blockID == 6) {
					if (lighting[rx][rz] > ry) { //not working?
						setBlock(rx,ry,rz,0);
						chunked_rerenderChunk(floor(rx/16), floor(rz/16), true);
						updateNeighbors(rx, rz);
					} else {
						placeTree(rx,ry,rz);	
						// this wont update chunks properly. duplicated because we need special chunk care here.
						chunked_rerenderChunk(floor(rx/16), floor(rz/16), true);
						updateNeighbors(rx, rz);
					}
				}
			}

			// Draw Clouds
			GRRLIB_SetTexture(tex_clouds, true);

			GX_Begin(GX_QUADS, GX_VTXFMT0, 4);

			GX_Position3f32(-worldX,worldY+3,-worldZ);
			GX_Color1u32(0xFFFFFFFF);
			GX_TexCoord2f32(cloudPos,0);
			GX_Position3f32(-worldX,worldY+3,worldZ*2);
			GX_Color1u32(0xFFFFFFFF);
			GX_TexCoord2f32(cloudPos,1);
			GX_Position3f32(worldX*2,worldY+3,worldZ*2);
			GX_Color1u32(0xFFFFFFFF);
			GX_TexCoord2f32(cloudPos+1,1);
			GX_Position3f32(worldX*2,worldY+3,-worldZ);
			GX_Color1u32(0xFFFFFFFF);
			GX_TexCoord2f32(cloudPos+1,0);

			GX_End();

			cloudPos = fmod(cloudPos + deltaTime/1000,1);

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
			GRRLIB_SetTexture(tex_terrain, false);
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

			int b;
			for (b = 0; b < 9; b++) {
				GRRLIB_DrawImg(b * 40 + 144, 442, tex_blockicons[thePlayer.inventory[b]], 0, 1, 1, 0xFFFFFFFF);
			}

			GRRLIB_DrawImg(thePlayer.inventory[9] * 40 + 136, 434, tex_inv_select, 0, 2, 2, 0xFFFFFFFF);

			if (thePlayer.select) {
				GRRLIB_Rectangle(80, 60, 480, 300, 0x0000007F, true);

				GXCraft_DrawText(224, 80, tex_font, "SELECT BLOCK");

				// Draw selection box (52x52)
				signed short sx, sy, sb;
				sx = ((short)(IR_0.sx) - 52) / 48 - 1;
				sy = ((short)(IR_0.sy) - 58) / 48 - 1;
				sb = sy * 9 + sx;
				if (sx >= 0 && sx < 9 && sy >= 0 && sb < 42)
					GRRLIB_Rectangle(sx * 48 + 98, sy * 48 + 104, 52, 52, 0xFFFFFF7F, true);

				int x, y;
				for (b = 0; b < 42; b++) {
					x = b % 9;
					y = floor(b/9);
					GRRLIB_DrawImg(x * 48 + 108, y * 48 + 114, tex_blockicons[inv_blocks[b]], 0, 1, 1, 0xFFFFFFFF);
				}
				GRRLIB_SetAntiAliasing(true);
				GRRLIB_DrawImg(IR_0.sx, IR_0.sy, tex_cursor, IR_0.angle, 1, 1, 0xFFFFFFFF);
				GRRLIB_SetAntiAliasing(false);

				if (WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_A) {
					if (sx >= 0 && sx < 9 && sy >= 0 && sb < 42)
						thePlayer.inventory[thePlayer.inventory[9]] = inv_blocks[sb];
					thePlayer.select = false;
					thePlayer.timer = 18;
				}
			}

			meminfo = mallinfo();
			int memusage = meminfo.uordblks;
			if (memusage > 0xE800000) // Correct gap between MEM2 and MEM2
				memusage -= 0xE800000;

			// Draw debugging elements
			GXCraft_DrawText(10,  25, tex_font, "FPS: %d", FPS);
			GXCraft_DrawText(10,  40, tex_font, "PX:% 7.2f", thePlayer.posX);
			GXCraft_DrawText(10,  55, tex_font, "PY:% 7.2f", thePlayer.posY);
			GXCraft_DrawText(10,  70, tex_font, "PZ:% 7.2f", thePlayer.posZ);
			GXCraft_DrawText(10,  85, tex_font, "LX:% 7.2f", thePlayer.lookX);
			GXCraft_DrawText(10, 100, tex_font, "LY:% 7.2f", thePlayer.lookY);
			GXCraft_DrawText(10, 115, tex_font, "LZ:% 7.2f", thePlayer.lookZ);
			GXCraft_DrawText(10, 130, tex_font, "DLSIZE: %i/%i (%i%%)", dluse, dlsize, dluse*100/dlsize);
			GXCraft_DrawText(10, 145, tex_font, "MEMUSAGE: %d (%dMB)", memusage, memusage/1048576);

			if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_2 && netcat_init) {
				GRRLIB_Screen2Texture(0, 0, tex_tmpscreen, false); // This is giving me the last content?
				GRRLIB_Screen2Texture(0, 0, tex_tmpscreen, false);
				netcat_log("-- START SCREENSHOT --\n");
				netcat_logf("W: %d, H: %d\n", tex_tmpscreen->w, tex_tmpscreen->h);
				scr_scanY = 0;
				status = SCREENSHOT;
			}

			GRRLIB_Render();
			FPS = CalculateFrameRate();
			break;
		case NUNCHUK:
			WPAD_ScanPads();

			data = WPAD_Data(WPAD_CHAN_0);
			if (data->exp.type == WPAD_EXP_NUNCHUK)
				status = INGAME;

			GRRLIB_3dMode(0.1, 1000, 45, 1, 0);

			if (getBlock(floor(thePlayer.posX),floor(thePlayer.posY+1.625),floor(thePlayer.posZ)) == 8) {
				GXColor c = {0x05, 0x05, 0x33};
				GX_SetFog(GX_FOG_LIN, 0, 32, 0.1, 1000, c);
			} else {
				GXColor c = {0x9E, 0xCE, 0xFF};
				GX_SetFog(GX_FOG_LIN, renderDistance - 32, renderDistance - chunkSize, 0.1, 1000, c);
			}

			GRRLIB_ObjectViewBegin();
			GRRLIB_ObjectViewTrans(-thePlayer.posX, -thePlayer.posY - 1.625, -thePlayer.posZ);
			GRRLIB_ObjectViewRotate(0, thePlayer.lookX, 0);
			GRRLIB_ObjectViewRotate(thePlayer.lookY, 0, 0);
			GRRLIB_ObjectViewEnd();

			//GRRLIB clears the vertex formats on mode switch
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA4, 0);
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);

			GRRLIB_SetTexture(tex_terrain, false);
			chunked_render(thePlayer);

			//Complain to user
			GRRLIB_2dMode();
			GRRLIB_Rectangle(0, 0, 640, 480, 0x0000007F, true);
			if (data->data_present == 0)
				GXCraft_DrawText(144, 232, tex_font, "PLEASE CONNECT WIIMOTE");
			else
				GXCraft_DrawText(144, 232, tex_font, "PLEASE CONNECT NUNCHUK");
			GRRLIB_Render();
			break;
		case SCREENSHOT:
			WPAD_ScanPads();

			if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_2) { // Cancel screenshot
				netcat_log("-- ABORT SCREENSHOT --\n");
				status = INGAME;
				break;
			}

			netcat_logf("%03d: ", scr_scanY);
			char c85[5];
			memset(c85, 0, sizeof c85);
			u32 lastcol = 0x00000000;
			int times = 1;
			int x;
			for (x = 0; x < tex_tmpscreen->w; x++) {
				u32 pixel = GRRLIB_GetPixelFromtexImg(x, scr_scanY, tex_tmpscreen);
				if (lastcol == 0x00000000) {
					lastcol = pixel;
					times = 1;
				} else if (pixel != lastcol) {
					color85(c85,lastcol);
					netcat_log(c85);
					if (times > 1) {
						netcat_logf("~%03d",times);
					}
					lastcol = pixel;
					times = 1;
				} else {
					times++;
				}
			}
			// Dump whats left
			color85(c85,lastcol);
			netcat_log(c85);
			if (times > 1) {
				netcat_logf("~%03d",times);
			}
			netcat_log("\n");
			scr_scanY++;
			if (scr_scanY >= tex_tmpscreen->h) {
				status = INGAME;
				netcat_log("-- END SCREENSHOT --\n");
			}

			// Draw fake scanner
			GRRLIB_2dMode();
			GRRLIB_DrawImg(0, 0, tex_tmpscreen, 0, 1, 1, 0xFFFFFFFF);
			GRRLIB_Line(0, scr_scanY, tex_tmpscreen->w, scr_scanY, 0xFF0000FF);
			GRRLIB_Render();
			break;
		}
		lastTime = thisTime;
	}
	netcat_log("ending...\n");
	netcat_close();

	GRRLIB_FreeTexture(tex_font);
	GRRLIB_FreeTexture(tex_inventory);
	GRRLIB_FreeTexture(tex_inv_select);
	GRRLIB_FreeTexture(tex_terrain);
	GRRLIB_FreeTexture(tex_cursor);
	GRRLIB_FreeTexture(tex_tmpscreen);

	GRRLIB_Exit();
	exit(0);
}

static void color85(char *buf, u32 color) {
	color >>= 8;
	int cnt;
	for (cnt = 3; cnt >= 0; cnt--) {
		unsigned char val = color % 85;
		color /= 85;
		buf[cnt] = val + 33;
	}
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

static int randnum(int x, int y) {
	double uni = ((double)rand())/((double)RAND_MAX);
	return floor(uni * (y - x + 1)) + x;
}
