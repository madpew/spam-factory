#ifndef _GFX_H
#define _GFX_H

#define FADE_DELAY 100
#define TYPE_DELAY 80

extern UINT8 bkg_tiles_offset;

void gfx_fadeout();
void gfx_fadein();

void gfx_load_tiles(unsigned char tileCount, unsigned char* tileData);
void gfx_load_map(unsigned char* mapData);

void gfx_load_hud(UINT8 tileCount, unsigned char* tileData, unsigned char* wndData);
void gfx_update_hud(UINT8 x, UINT8 len, unsigned char* data);

void gfx_write(char* string, UINT8 x, UINT8 y, UINT8 w); 
void gfx_type(char* string, UINT8 x, UINT8 y, UINT8 w); 
void gfx_clear(UINT8 x, UINT8 y, UINT8 w, UINT8 h);
void gfx_scroller(UINT8 x, UINT8 y, char* string, UINT8 len, UINT8 w, UINT8 scroll);

#endif /* _GFX_H */