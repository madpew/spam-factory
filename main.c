/*
	SPAM Factory Inc.
	A game by madpew for open jam 2018
	Licensed under BSD 3 Clause License
	github: https://github.com/madpew/spam-factory
*/

#include <gb/gb.h>
#include <gb/rand.h>

#include "timer.h"
#include "music.h"
#include "gamestate.h"

void main() {

	initrand(DIV_REG);
	
	// Set WAV Ram before enabling sound
	//($ff30-$ff3f).
	*(UINT8*)0xFF30ul = 0xFFu; 
	*(UINT8*)0xFF31ul = 0xEEu;
	*(UINT8*)0xFF32ul = 0xDDu;
	*(UINT8*)0xFF33ul = 0xCCu;
	*(UINT8*)0xFF34ul = 0xBBu;
	*(UINT8*)0xFF35ul = 0xAAu;
	*(UINT8*)0xFF36ul = 0x99u;
	*(UINT8*)0xFF37ul = 0x88u;
	*(UINT8*)0xFF38ul = 0x00u;
	*(UINT8*)0xFF39ul = 0x00u;
	*(UINT8*)0xFF3Aul = 0x11u;
	*(UINT8*)0xFF3Bul = 0x11u;
	*(UINT8*)0xFF3Cul = 0x00u;
	*(UINT8*)0xFF3Dul = 0x00u;
	*(UINT8*)0xFF3Eul = 0x11u;
	*(UINT8*)0xFF3Ful = 0x11u;
	
	
	NR52_REG = 0x8Fu;
	NR51_REG = 0xFFu;
	NR50_REG = 0x77u;

	music_init();
	timer_init();
	
	DISPLAY_ON;
	SPRITES_8x16;
	SHOW_BKG;

	Gamestate = GS_NONE;
	gamestate_set(GS_PREROLL);
	
	while(1)
	{
		wait_vbl_done();
		gamestate_tick();
		timer_clear(TIMERFLAG_ANIMATION);
	}
}