#include <gb/gb.h>
#include <gb/rand.h>

#include "gamestate.h"
#include "gfx.h"
#include "sfx.h"
#include "music.h"
#include "timer.h"

#include "datainc/map_hud.h"
#include "datainc/map_all.h"

#include "datainc/strings.h"

#include "datainc/sprites.h"

#include "datainc/mus_pushing.h"
#include "datainc/mus_menu.h"
#include "datainc/mus_email.h"
#include "datainc/mus_intro.h"
#include "datainc/mus_game.h"
#include "datainc/mus_game1.h"
#include "datainc/mus_game2.h"




////////////////////////////////////////////////////////////////

void menu_enter();
void menu_tick();

void preroll_enter();
void preroll_tick();

void intro_enter();
void intro_tick();

void gameover_enter();
void gameover_tick();

void win_enter();
void win_tick();

void email_enter();
void email_tick();
void show_email(char* from, char* text);

void info_enter();
void info_tick();
void show_info(char* text);

void offer_enter();
void offer_tick();
void show_offer(char* text, UINT16 price, UINT8 offerId);

void game_enter();
void game_tick();

////////////////////////////////////////////////////////////////

UINT8 Gamestate;
UINT8 Laststate;

UINT8 WaitForInput;

void gamestate_set(UINT8 state)
{
	if (state == Gamestate) return;
	
	if (Gamestate != GS_NONE) gfx_fadeout();
	
	Laststate = Gamestate;
	Gamestate = state;
	
	switch(Gamestate)
	{
		case GS_PREROLL: preroll_enter(); break;
		case GS_MENU: menu_enter(); break;
		case GS_INTRO: intro_enter(); break;
		case GS_EMAIL: email_enter(); break;
		case GS_INFO: info_enter(); break;
		case GS_PLAYING: game_enter(); break;
		case GS_OFFER: offer_enter(); break;
		case GS_WIN: win_enter(); break;
		case GS_GAMEOVER: gameover_enter(); break;
	}
	
	gfx_fadein();
}

void gamestate_tick()
{
	switch(Gamestate)
	{
		case GS_PREROLL: preroll_tick(); break;
		case GS_MENU: menu_tick(); break;
		case GS_INTRO: intro_tick(); break;
		case GS_PLAYING: game_tick(); break;
		case GS_OFFER: offer_tick(); break;
		case GS_EMAIL: email_tick(); break;
		case GS_INFO: info_tick(); break;
		case GS_GAMEOVER: gameover_tick(); break;
		case GS_WIN: win_tick(); break;
	}
}

////////////////////////////////////////////////////////////////
// BOOT SCREEN
////////////////////////////////////////////////////////////////

void preroll_enter()
{
	gfx_clear(0, 0, 20, 18);
	wait_vbl_done();
	gfx_load_tiles(110, map_all_tiles);
	gfx_load_hud(spamhud);
	WaitForInput = 0;
}

void preroll_tick()
{
	gfx_type(TITLE_PREROLL, 2,7,16);
	delay(300);
	gamestate_set(GS_PLAYING);
	//gamestate_set(GS_MENU);
}

////////////////////////////////////////////////////////////////
// MENU SCREEN
////////////////////////////////////////////////////////////////

void menu_enter()
{
	music_play(MUS_GAME2);
	gfx_load_map(title_map);
	delay(300);
	HIDE_SPRITES;
}

UINT8 scroller_offset;

void menu_tick()
{
	WaitForInput = 1;
	gfx_scroller(5, 16, "            HELLO EVERYONE, MADPEW HERE. THANKS FOR TRYING OUT MY SUBMISSION FOR OPEN JAM 2018! FEEDBACK IS VERY WELCOME. @ ME ON TWITTER. SPECIAL THANKS TO EVERYONE ON THE GBDEV DISCORD. COULD NOT MAKE THIS WITHOUT YOU!", 220, 12, scroller_offset);
	
	if (timer_check(TIMERFLAG_ANIMATION))
	{
		scroller_offset++;
	}
	
	if (joypad() & J_START)
	{
		gamestate_set(GS_INTRO);
	}
}

////////////////////////////////////////////////////////////////
// INTRO SCREEN
////////////////////////////////////////////////////////////////

void intro_enter()
{
	WaitForInput = 0;
	music_play(MUS_INTRO);
	gfx_load_map(intro_map);
	HIDE_WIN;
	HIDE_SPRITES;
}

void intro_tick()
{
	if (WaitForInput == 0)
	{
		gfx_type(STR_INTRO_1,2,2,16);
		gfx_write(STR_LETSGO, 4,14, 8);
		WaitForInput = 1;
	}

	if (joypad())
	{
		gamestate_set(GS_PLAYING);
	}
}

////////////////////////////////////////////////////////////////
// GAMEOVER SCREEN
////////////////////////////////////////////////////////////////

void gameover_enter()
{
	WaitForInput = 0;
	music_play(MUS_GAME2);
	gfx_load_map(intro_map);
	HIDE_WIN;
	HIDE_SPRITES;
}

void gameover_tick()
{
	if (WaitForInput == 0)
	{
		gfx_type("GAMEOVER MAN,\nGAMEOVER.\nHOW COULD YOU\nMESS THIS 1 UP?\n\nLIKE SERIOUSLY.\n\nBETTER LUCK NEXT\nTIME, I GUESS.\0",2,2,16);
		gfx_write("PRESS\nANY KEY\0", 4,14, 8);
		WaitForInput = 1;
	}

	if (joypad())
	{
		gamestate_set(GS_MENU);
	}
}

////////////////////////////////////////////////////////////////
// WIN SCREEN
////////////////////////////////////////////////////////////////

void win_enter()
{
	WaitForInput = 0;
	music_play(MUS_WIN);
	gfx_load_map(win_map);
	HIDE_WIN;
	HIDE_SPRITES;
}

void win_tick()
{
	if (WaitForInput == 0)
	{
		gfx_type("YOU'VE MADE IT!\nYOU ARE THE\nKING OF SPAM,\nSWIMMING IN CASH\nNOT HAVING TO DO\nANY WORK.\nWELL PLAYED!",2,9,16);
		WaitForInput = 1;
	}

	if (joypad() & J_START)
	{
		gamestate_set(GS_MENU);
	}
}

////////////////////////////////////////////////////////////////
// EMAIL SCREEN
////////////////////////////////////////////////////////////////

char* emailFrom;
char* emailText;

void show_email(char* from, char* text)
{
	emailFrom = from;
	emailText = text;
	gamestate_set(GS_EMAIL);
}

void email_enter()
{
	WaitForInput = 0;
	music_play(MUS_GAME2);
	gfx_load_map(mail_map);
	gfx_write(emailFrom, 6,3, 13);
	gfx_write(emailText, 1,5, 18);
	WaitForInput = 1;
	HIDE_WIN;
	HIDE_SPRITES;
}

void email_tick()
{
	if (joypad())
	{
		gamestate_set(GS_PLAYING); // SHOULD BE PLAYING
	}
}

////////////////////////////////////////////////////////////////
// INFO SCREEN
////////////////////////////////////////////////////////////////

char* infoText;

void show_info(char* text)
{
	infoText = text;
	gamestate_set(GS_INFO);
}

void info_enter()
{
	music_play(MUS_GAME2);
	gfx_load_map(intro_map);
	WaitForInput = 0;
	HIDE_WIN;
	HIDE_SPRITES;
}

void info_tick()
{
	if (WaitForInput == 0)
	{
		gfx_type(infoText,2,2,16);
		gfx_write("THANKS\nFOR INFO\0", 4,14, 8);		
		WaitForInput = 1;
	}
	
	if (joypad())
	{
		gamestate_set(GS_PLAYING);
	}
}

////////////////////////////////////////////////////////////////
// OFFER SCREEN
////////////////////////////////////////////////////////////////

char* offerText;
UINT8 offerId;
UINT16 offerPrice;
UINT8 offerAccepted;

void show_offer(char* text, UINT16 price, UINT8 offerId)
{
	offerAccepted = 0;
	offerText = text;
	gamestate_set(GS_OFFER);
}

void offer_enter()
{
	WaitForInput = 0;
	music_play(MUS_GAME2);
	gfx_load_map(intro_map);
	HIDE_WIN;
	HIDE_SPRITES;
}

void offer_tick()
{
	if (WaitForInput == 0)
	{
		gfx_type(offerText,2,2,16);
		gfx_write(" ACCEPT\n REJECT\0", 4,14, 8);		
		// TODO: Enable Cursor Sprite
		WaitForInput = 1;
	}
	
	// do select logic here
	/*if (joypad())
	{
		gamestate_set(GS_PLAYING);
	}*/
}

////////////////////////////////////////////////////////////////
// GAME SCREEN
////////////////////////////////////////////////////////////////

#define SPR_CURSOR 0
#define SPR_ICON_EMAIL0 1
#define SPR_ICON_EMAIL1 2
#define SPR_ICON_OFFER0 3 
#define SPR_ICON_OFFER1 4

#define CUR_BLACK 0
#define CUR_WHITE 2
#define CUR_GRAY 4
#define CUR_GHOST 6

#define MENU_MAIN 0
#define MENU_WORK 1
#define MENU_STATS 2
#define MENU_UPGRADES 3

// PLAYER Vars
UINT16 p_money;
UINT16 p_emails;
UINT16 p_targets;


// SIM Vars
INT8 manual_mail = 0;
INT8 cost_per_tick = 0;
INT8 mails_per_tick = 0;
INT8 targets_per_tick = 0;
INT8 scam_amount = 0;
INT8 scam_success = 0;

// UI Vars
UINT8 menu_row;
UINT8 menu_rows;

UINT8 ui_icon_toggle;

UINT8 ui_has_email;
UINT8 ui_has_offer;

UINT8 ui_menu;
UINT8 ui_update_required = 0;

void switch_screen(UINT8 screen);


void handle_click()
{
	if (ui_menu == MENU_MAIN)
	{
		switch(menu_row)
		{
			case 0: switch_screen(MENU_WORK); return;
			case 1:{ui_has_email = 1; switch_screen(MENU_MAIN); return;} // upgrades
			case 2: return; // report
			case 3: if (ui_has_email) { show_email("TEST\0", "REALLY\0"); }
		}
	}
}

void switch_screen(UINT8 screen)
{
		gfx_fadeout();
		
		ui_menu = screen;
		ui_icon_toggle = 0;
		
		move_sprite(0, 20, 48-4);
		menu_row = 0;
	
		if (ui_menu == MENU_MAIN)
		{
			gfx_write("MY COMPUTER\0",2,1,20);
			
			gfx_write("DO WORK\0",3,4,16);
			gfx_write("UPGRADES\0",3,5,16);
			gfx_write("CHECK REPORT\0",3,6,16);
			
			menu_rows = 3;
			
			if (ui_has_email) 
			{
				gfx_write("CHECK MAIL\0",3,7,16);
				menu_rows++;
			}
			
			if (ui_has_offer)
			{
				gfx_write("MEET WITH &&&&&\0",3,8,16);
				menu_rows++;
			}
		}
		
		if (ui_menu == MENU_WORK)
		{
			gfx_write("WORKBENCH\0",2,1,20);
			
			gfx_write("FIND TARGET\0",3,4,16);
			gfx_write("SEND MAIL\0",3,5,16);
			gfx_write("SCRIPT AUTOMAILER\0",3,6,16);
			gfx_write("SCRIPT AUTOSCRAPER\0",3,6,16);
			gfx_write("HACK MAILSERVER\0",3,6,16);
			gfx_write("...\0",3,6,16);
			
			menu_rows = 3;
		}
		
		gfx_fadein();
}

void select()
{
	scroll_sprite(SPR_CURSOR,1,0);
	set_sprite_tile(SPR_CURSOR,CUR_GRAY);
	delay(25);
	scroll_sprite(SPR_CURSOR,1,1);
	set_sprite_tile(SPR_CURSOR,CUR_WHITE);
	delay(25);
	scroll_sprite(SPR_CURSOR,1,0);
	delay(25);
	scroll_sprite(SPR_CURSOR,-1,0);
	delay(25);
	scroll_sprite(SPR_CURSOR,-1,-1);
	set_sprite_tile(SPR_CURSOR,CUR_GRAY);
	delay(25);
	scroll_sprite(SPR_CURSOR,-1,0);
	set_sprite_tile(SPR_CURSOR,CUR_BLACK);
}

void game_enter()
{
	gfx_load_map(game_map);
	music_play(MUS_MENU);

	// setup sprites
	set_sprite_data(0, 16, sprites_tiles);
	set_sprite_tile(SPR_CURSOR,0);
	set_sprite_tile(SPR_ICON_EMAIL0, 8);
	set_sprite_tile(SPR_ICON_EMAIL1, 10);
	set_sprite_tile(SPR_ICON_OFFER0, 12);
	set_sprite_tile(SPR_ICON_OFFER1, 14);
	
	switch_screen(MENU_MAIN);
	
	// check if we're coming from an offer
	if (Laststate == GS_INTRO || Laststate == GS_PREROLL)
	{
		// new game, init all vars
		ui_has_email = 0;
		ui_has_offer = 0;
		
		p_money = 100;
		p_targets = 0;
		p_emails = 0;
	}
	
	if (Laststate == GS_EMAIL)
	{
		ui_has_email = 0;
	}
	
	if (Laststate == GS_OFFER || Laststate == GS_INFO)
	{
		// process result
		ui_has_offer = 0;
	}
	
	SHOW_SPRITES;
	SHOW_WIN;
}




void hud_update(UINT8 x, UINT16 number)
{
	char chars[4];
	UINT16 value = number;
	
	if (value > 9999)
	{
		value = value / 1000;
		chars[3] = 11; // "k"
		chars[2] = (value % 10);
		chars[1] = ((value/10) % 10);
		chars[0] = 10; // " "
	}
	else
	{
		chars[3] = (value % 10);
		chars[2] = ((value/10) % 10);
		chars[1] = ((value/100) % 10);
		chars[0] = ((value/1000) % 10);
	}
	
	gfx_update_hud(x, 4, chars);
}

void simulation_tick()
{
}

void game_tick()
{
	ui_update_required = 0;

	if (timer_check(TIMERFLAG_ICONS))
	{
		timer_clear(TIMERFLAG_ICONS);
		ui_icon_toggle++;
		
		move_sprite(SPR_ICON_EMAIL0, (ui_icon_toggle & ui_has_email) * 140, 124);
		move_sprite(SPR_ICON_EMAIL1, (ui_icon_toggle & ui_has_email) * 148, 124);
		
		move_sprite(SPR_ICON_OFFER0, (ui_icon_toggle & ui_has_offer) * 120, 124);
		move_sprite(SPR_ICON_OFFER1, (ui_icon_toggle & ui_has_offer) * 128, 124);
	}

	if (timer_check(TIMERFLAG_GAMESTEP))
	{
		timer_clear(TIMERFLAG_GAMESTEP);
		
		// update stuff
		simulation_tick();
		
		// win and loose condition
		
		if (p_money >= 100) // change to: harvested everything or 65k?
		{
			gamestate_set(GS_WIN);
		}
		
		if (p_money == 0)
		{
			gamestate_set(GS_GAMEOVER);
		}
		
		manual_mail = 0;
		
		ui_update_required = 1;
	}
	
	if (ui_update_required != 0)
	{
		hud_update(2, p_money);
		hud_update(9, p_emails);
		hud_update(16, p_targets);
	}
	
	UINT8 joy = joypad();
	
	if(joy & J_DOWN && menu_row < (menu_rows-1))
	{
		menu_row++;
		scroll_sprite(SPR_CURSOR,0,1);
		
		for(int i=1; i!=8; i++)
		{
			delay(4+i*i);
			scroll_sprite(SPR_CURSOR,0,1);
		}
	}
	
	if(joy & J_UP && menu_row != 0)
	{
		menu_row--;
		scroll_sprite(SPR_CURSOR,0,-1);
		
		for(int i=1; i!=8; i++)
		{
			delay(4+i*i);
			scroll_sprite(SPR_CURSOR,0,-1);
		}
	}
	
	if (joy & J_SELECT)
	{
		switch_screen(MENU_MAIN);
	}
	
	if (joy & J_A || joy & J_B)
	{
		select();
		handle_click();
	}
}

