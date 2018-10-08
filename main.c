/*
	SPAM Factory Inc.
	A game by madpew for open jam 2018
	Licensed under <LICENSE>
	github: ...
*/

#include <gb/gb.h>
#include <gb/rand.h>

#include "timer.h"
#include "music.h"
#include "sfx.h"
#include "gfx.h"
#include "gamestate.h"

void main() {

	initrand(DIV_REG);
	
	gfx_fadeout();
	
	NR52_REG = 0x8Fu;
	NR51_REG = 0xFFu;
	NR50_REG = 0x77u;

	music_init();
	timer_init();
	
	DISPLAY_ON;
	SPRITES_8x16;
	SHOW_SPRITES;
	SHOW_BKG;

	// setup cursor sprite
	// set_sprite_data(0, 8, Walker_tiles);
	// set_sprite_tile(0,0);
	
	Gamestate = GS_NONE;
	gamestate_set(GS_PREROLL);
	
	while(1)
	{
		gamestate_tick();
		
		timer_clear(TIMERFLAG_ANIMATION);
		
		wait_vbl_done();
		
		// blinking cursor
		//if (timer_check(TIMERFLAG_ANIMATION))
		//{
			//timer_clear(TIMERFLAG_ANIMATION);
			//playerAnim++;
			//set_sprite_tile(0,0 + (playerAnim & 0x01u)*4);
		//}
		
		// check timer for gameplay ticks
		
	}
}

/*

void handle_input()
{
//	move_bkg(CAM[0], CAM[1]);
//	move_sprite(0, SPR[0], SPR[1]);
//	move_sprite(1, SPR[0]+8, SPR[1]);
}*/