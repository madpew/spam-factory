#include <gb/gb.h>

#include "gfx.h"
#include "sfx.h"

/* PALETTE TRICKERY (fade out/in etc.) */ 

UINT8 BKG_PALETTE;
UINT8 OBJ_PALETTE0;
UINT8 OBJ_PALETTE1;

UINT8 GFX_FADED;

void gfx_fadeout()
{
	if (GFX_FADED == 1u) return;
	
	BKG_PALETTE = BGP_REG;
	OBJ_PALETTE0 = OBP0_REG;
	OBJ_PALETTE1 = OBP1_REG;
	GFX_FADED = 1u;
	
	// clear lowest color bit
	BGP_REG = BGP_REG & 0xAAu;
	OBP0_REG = OBP0_REG & 0xAAu;
	OBP1_REG = OBP1_REG & 0xAAu;
	
	delay(FADE_DELAY);
	
	// shift bits
	BGP_REG = BGP_REG >> 1;
	OBP0_REG = OBP0_REG >> 1;
	OBP1_REG = OBP1_REG >> 1;
	
	delay(FADE_DELAY);
	
	BGP_REG = 0;
	OBP0_REG = 0;
	OBP1_REG = 0;
}

void gfx_fadein()
{
	if (GFX_FADED == 0u) return;
	
	GFX_FADED = 0u;
	// only upper bits in lower
	BGP_REG = (BKG_PALETTE & 0xAAu) >> 1 ;
	OBP0_REG = (OBJ_PALETTE0 & 0xAAu) >> 1 ;
	OBP1_REG = (OBJ_PALETTE1 & 0xAAu) >> 1 ;
	
	delay(FADE_DELAY);
	
	// only upper bits
	BGP_REG = BKG_PALETTE & 0xAAu;
	OBP0_REG = OBJ_PALETTE0 & 0xAAu;
	OBP1_REG = OBJ_PALETTE1 & 0xAAu;
	
	delay(FADE_DELAY);
	
	BGP_REG = BKG_PALETTE;
	OBP0_REG = OBJ_PALETTE0;
	OBP1_REG = OBJ_PALETTE1;
}

/* hud */

void gfx_load_hud(UINT8 tileCount, unsigned char* tileData, unsigned char* wndData)
{
	set_win_data(bkg_tiles_offset, tileCount, tileData);
	
	UINT8 wndDataCopy[20];
	
	for(UINT8 i=0; i<20; i++)
	{
		wndDataCopy[i] = wndData[i];
		wndDataCopy[i] += bkg_tiles_offset;
	}
	
    set_win_tiles(0, 0, 20, 1, wndDataCopy);
	
    move_win(7, 136);
}

void gfx_update_hud(UINT8 x, UINT8 len, unsigned char* data)
{
	for(UINT8 i=0; i<len; i++)
	{
		data[i] += bkg_tiles_offset;
	}
	
    set_win_tiles(x, 0, len, 0, data);
}

/*
 write a string to the background
*/

void gfx_type(char* string, UINT8 x, UINT8 y, UINT8 w)
{
	char converted[360];
	char current;
	UINT8 len = 0u;
	UINT8 i = 0;
	UINT8 cx;
	UINT8 cy;
	
	while(1)
	{
		current = string[len];
		if (current == '\0') break;
		converted[len] = current - 32;
		if (current == '\n') converted[len] = 0xFF;
		len++;
	}
	
	cx = x;
	cy = y;
	
	for(i=0; i<len; i++)
	{
		if (converted[i] == 0xFF) { cx = x; cy++; continue; }
		if (cx >= x+w) { cx = x; cy++; }
		set_bkg_tiles(cx, cy, 1, 1, &converted[i]);
		cx++;
		
		if (string[i] != ' ') 
			sfx_type();
		
		delay(TYPE_DELAY);
	}
}

void gfx_write(char* string, UINT8 x, UINT8 y, UINT8 w)
{
	char converted[360];
	char current;
	UINT8 len = 0u;
	UINT8 i = 0;
	UINT8 cx;
	UINT8 cy;
	
	while(1)
	{
		current = string[len];
		if (current == '\0') break;
		converted[len] = current - 32;
		if (current == '\n') converted[len] = 0xFF;
		len++;
	}
	
	cx = x;
	cy = y;
	
	for(i=0; i<len; i++)
	{
		if (converted[i] == 0xFF) { cx = x; cy++; continue; }
		if (cx >= x+w) { cx = x; cy++; }
		set_bkg_tiles(cx, cy, 1, 1, &converted[i]);
		cx++;
	}
}

void gfx_scroller(UINT8 x, UINT8 y, char* string, UINT8 len, UINT8 w, UINT8 scroll)
{
	char converted[20];
	char current;
	UINT8 i = 0;
	for(i=scroll; i<scroll+w; i++)
	{
		current = string[i % len];
		converted[i-scroll] = current - 32;
	}
	
	set_bkg_tiles(x, y, w, 1, converted);
}

void gfx_clear(UINT8 x, UINT8 y, UINT8 w, UINT8 h)
{
	char converted[32];
	
	for (int j=0; j<32; j++)
		converted[j] = 0;
	
	for (int i=0; i<h; i++)
	{
		set_bkg_tiles(x, y+i, w, 1, converted);
	}
}

unsigned char bkg_tiles_offset;

void gfx_load_tiles(unsigned char tileCount, unsigned char* tileData)
{
	bkg_tiles_offset = tileCount;
	set_bkg_data(0u, tileCount, tileData);
}

void gfx_load_map(unsigned char* mapData)
{
	set_bkg_tiles(0u, 0u, 20, 18, mapData);
}