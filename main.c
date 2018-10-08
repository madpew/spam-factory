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
		gamestate_tick();
		timer_clear(TIMERFLAG_ANIMATION);
	}
}