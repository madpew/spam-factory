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

#include "datainc/mus_menu.h" // MENU
#include "datainc/mus_email.h" // EMAIL
#include "datainc/mus_game.h"	// info
#include "datainc/mus_game1.h" // WIN
#include "datainc/mus_game2.h" // GAME
#include "datainc/mus_intro.h" // gameover

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
void recv_email(char* from, char* text);
void show_email();

void info_enter();
void info_tick();
void show_info(char* text);

void game_enter();
void game_tick();

UINT8 ui_has_email;
UINT16 STAT_mails_sent;
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
	gamestate_set(GS_MENU);
}

////////////////////////////////////////////////////////////////
// MENU SCREEN
////////////////////////////////////////////////////////////////

void menu_enter()
{
	music_play(MUS_MENU);
	gfx_load_map(title_map);
	delay(300);
	HIDE_SPRITES;
}

UINT8 scroller_offset;

void menu_tick()
{
	WaitForInput = 1;
	gfx_scroller(5, 16, "            HI THERE, WELCOME TO MY OPENJAM 2018 ENTRY! SPAM ME: @MADPEW ON TWITTER\0", 83, 12, scroller_offset);
	
	if (timer_check(TIMERFLAG_ANIMATION))
	{
		scroller_offset = (scroller_offset+1)%83;
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
	music_play(MUS_GAME);
	gfx_load_map(intro_map);
	HIDE_WIN;
	HIDE_SPRITES;
}

void intro_tick()
{
	if (WaitForInput == 0)
	{
		gfx_type("HEY YOU BORROWED\n64K$ FROM US AND\nWE WANT IT BACK.\nBETTER GO EARN\nSOME MONEY!",2,2,16);
		delay(1000);
		gfx_clear(2,2,16,9);
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
	music_play(MUS_INTRO);
	gfx_load_map(intro_map);
	HIDE_WIN;
	HIDE_SPRITES;
}

void gameover_tick()
{
	if (WaitForInput == 0)
	{
		gfx_type("GAMEOVER MAN,\nGAMEOVER.\nYOU RAN OUT OF\nFUNDS!\n\nLIKE SERIOUSLY.\n\nBETTER LUCK NEXT\nTIME, I GUESS.\0",2,2,16);
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

void recv_email(char* from, char* text)
{
	emailFrom = from;
	emailText = text;
	ui_has_email = 1;
}

void show_email()
{
	gamestate_set(GS_EMAIL);
	ui_has_email = 0;
}

void email_enter()
{
	WaitForInput = 0;
	music_play(MUS_EMAIL);
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
	music_play(MUS_GAME);
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

UINT8 p_ram; // how many apps running
UINT8 p_pc_costs = 1;
UINT8 p_rent = 1;

// SIM Vars
#define PRICE_CPU 300
#define PRICE_RAM 120
#define PRICE_NET 80
#define PRICE_SCRAPER 20
#define PRICE_MAILER 20

UINT8 manual_scrape = 0;
UINT8 manual_mail = 0;

UINT8 automailer_count = 0;
UINT8 automailer_mult = 1;
UINT8 automailer_costs = 3;

UINT8 autoscraper_count = 0;
UINT8 autoscraper_mult = 1;
UINT8 autoscraper_costs = 4;

UINT16 cost_per_tick = 0;
UINT16 mails_per_tick = 0;
UINT16 targets_per_tick = 0;

UINT8 scam_amount = 2; // cash per scam
UINT8 scam_success = 0; // success rate of scamming

// UI Vars
UINT8 menu_row;
UINT8 menu_rows;

UINT8 ui_icon_toggle;



UINT8 ui_menu;
UINT8 ui_update_required = 0;

void switch_screen(UINT8 screen, UINT8 fade);

void handle_click()
{
	if (ui_menu == MENU_MAIN)
	{
		switch(menu_row)
		{
			case 0: switch_screen(MENU_WORK,1); return;
			case 1: switch_screen(MENU_UPGRADES,1); return;
			case 2: if (ui_has_email) show_email(); return;
		}
	}
	else if (ui_menu == MENU_WORK)
	{
		switch(menu_row)
		{
			case 0: manual_scrape++; return;
			case 1: {
				manual_mail++;
				
				if ((_rand() & 0xF) == 0)
				{
					
					switch(_rand() & 0x07)
					{
						case 0: recv_email("ARG@LOST.%%\0","PLEASE STOP.\nI HAVE NO USE\nFOR ENLARGEMENT.\nSERIOUSLY\nSTOP IT\0"); break;
						case 1: recv_email("MI%@&&.COM\0","THANK YOU FOR\nTHE KIND OFFER!\nI WILL TAKE 3 OF\nTHE RED ONES!\n\0"); break;
						case 2: recv_email("LISA@M&.RU\0","HEJ!\nI WANNA JOIN!\nTHE VENTURE LOOK\nPROMISING!!1\nWHERE DO I SIGN\nUP?\n\0"); break;
						case 3: recv_email("PEW@GM&IL.COM\0","DONT UPLOAD THE VIDEO! I'LL PAY THE BITCOINS!! HOW DO I DO THIS?\0"); break;
						case 4: recv_email("S&F@&.DE\0","YES SURE!\nI WANNA BUY SOME\nBITCOINS FROM\nYOUR SITE!\nIS THERE A\nPROMOCODE?\0"); break;
						case 5: recv_email("M&Z@OU&.UK\0","GET LOST!!!!!!!\0"); break;
						case 6: recv_email("A&S&@G&.NET\0","NO,\nI WONT PAY FOR YOU TO STOP\0"); break;
						case 7: recv_email("R&6@GM&.AT\0","DEAR PRINCE\nJEFFREY.\nI WILL HELP YOU!\nWHERE SHOULD I\nSEND THE MONEY?\0"); break;
					}
				}
				return;
			}
			case 2: 
			{
				if (p_money > PRICE_SCRAPER && p_ram > (automailer_count + autoscraper_count))
				{
					autoscraper_count++;
					p_money -= PRICE_SCRAPER;
					sfx_upgrade();
					if (p_ram == (automailer_count + autoscraper_count)) show_info("HEY YOU,\nSEEMS YOUR RAM\nIS FULL!\nTRY UPGRADING\nYOUR PC.");
				}
				else
				{
					sfx_fail();
				}	
				return;
			}
			case 3: 
			{
				if (p_money > PRICE_MAILER && p_ram > (automailer_count + autoscraper_count))
				{
					automailer_count++;
					p_money -= PRICE_MAILER;
					sfx_upgrade();
					if (p_ram == (automailer_count + autoscraper_count)) show_info("HEY YOU,\nSEEMS YOUR RAM\nIS FULL!\nTRY UPGRADING\nYOUR PC.");
				}
				else
				{
					sfx_fail();
				}
				return;
			}
		}
	}
	else if (ui_menu == MENU_UPGRADES)
	{
		switch(menu_row)
		{
			case 0: // upgrade cpu
			{
				if (p_money > PRICE_CPU)
				{
					autoscraper_mult+=3;
					p_pc_costs++;
					p_money -= PRICE_CPU;
					sfx_upgrade();
				}
				else
				{
					sfx_fail();
				}
				return;
			}
			case 1: // upgrade ram
			{
				if (p_money > PRICE_RAM)
				{
					p_ram+=4;
					p_money -= PRICE_RAM;
					sfx_upgrade();
				}
				else
				{
					sfx_fail();
				}
				return;
			}
			case 2: // upgrade net
			{
				if (p_money > PRICE_NET)
				{
					automailer_mult+=3;
					p_money -= PRICE_NET;
					sfx_upgrade();
				}
				else
				{
					sfx_fail();
				}
				return;
			}
		}
	}
}

void switch_screen(UINT8 screen, UINT8 fade)
{
		if (fade != 0) gfx_fadeout();
		
		ui_menu = screen;
		
		if (fade != 0) 
		{
			move_sprite(0, 20, 48-4);
			menu_row = 0;
		
			gfx_clear(2,1,17,1);
		}
		
		gfx_clear(3,4,16,9);
		
		if (ui_menu == MENU_MAIN)
		{
			gfx_write("MY COMPUTER\0",2,1,19);
			
			gfx_write("DO WORK\0",3,4,16);
			gfx_write("UPGRADE PC\0",3,5,16);
			
			menu_rows = 2;
			
			if (ui_has_email) 
			{
				gfx_write("READ MAIL\0",3,6,16);
				menu_rows++;
			}
		}
		
		if (ui_menu == MENU_WORK)
		{
			gfx_write("WORKBENCH\0",2,1,20);
			
			gfx_write("FIND TARGET\0",3,4,16);
			gfx_write("SEND MAIL\0",3,5,16);
			menu_rows = 2;
			
			if (p_money > PRICE_SCRAPER && p_ram > (automailer_count + autoscraper_count))
			{
				gfx_write("BUY SCRAPER  20$\0",3,6,16);
				menu_rows++;
			}
			
			if (p_money > PRICE_MAILER && p_ram > (automailer_count + autoscraper_count))
			{
				gfx_write("BUY MAILER   20$\0",3,7,16);
				menu_rows++;
			}
		}
		
		if (ui_menu == MENU_UPGRADES)
		{
			gfx_write("UPGRADE PC\0",2,1,20);
			
			menu_rows = 0;
			
			
			gfx_write("BETTER CPU  300$\0",3,4,16);
			menu_rows++;
			
			
			gfx_write("ADD RAM     120$\0",3,5,16);
			menu_rows++;
			
			
			gfx_write("BANDWIDTH   80$\0",3,6,16);
			menu_rows++;
			
		}
		
		if (fade != 0) gfx_fadein();
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
	music_play(MUS_GAME2);

	// setup sprites
	set_sprite_data(0, 16, sprites_tiles);
	set_sprite_tile(SPR_CURSOR,0);
	set_sprite_tile(SPR_ICON_EMAIL0, 8);
	set_sprite_tile(SPR_ICON_EMAIL1, 10);
	set_sprite_tile(SPR_ICON_OFFER0, 12);
	set_sprite_tile(SPR_ICON_OFFER1, 14);
	
	switch_screen(MENU_MAIN,1);
	
	// check if we're coming from an offer
	if (Laststate == GS_INTRO || Laststate == GS_PREROLL)
	{
		// new game, init all vars
		ui_has_email = 0;
		p_pc_costs = 1;
		p_money = 100;
		p_targets = 0;
		p_emails = 0;
		p_ram = 4;
		p_pc_costs=1;
		automailer_count = 0;
		automailer_mult = 1;
		autoscraper_count = 0;
		autoscraper_mult = 1;
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
	UINT16 money_gain = 0;
	mails_per_tick = manual_mail + automailer_count * 2 * automailer_mult;
	targets_per_tick = manual_scrape + autoscraper_count * 3 * autoscraper_mult;
	cost_per_tick = p_pc_costs + automailer_count * automailer_costs + autoscraper_count * autoscraper_costs;

	// scrape targets
	p_targets += targets_per_tick;
	
	if (mails_per_tick > p_targets)
	{
		p_emails = p_targets;
		p_targets = 0;
	}
	else
	{
		p_emails = mails_per_tick;
		p_targets -= mails_per_tick;
	}
	
	if (cost_per_tick > p_money)
	{
		p_money = 0;
	}
	else
	{
		p_money -= cost_per_tick;
	}
	
	STAT_mails_sent += p_emails;
	money_gain = p_emails * (_rand() & 0x07);
	
	if (money_gain + p_money < p_money)
	{
		p_money = 0xFFFFul;
	}
	else
	{
		p_money += money_gain;
	}
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
	}

	if (timer_check(TIMERFLAG_GAMESTEP))
	{
		timer_clear(TIMERFLAG_GAMESTEP);
		
		// update stuff
		simulation_tick();
		
		// win and loose condition
		
		if (p_money >= 64000)
		{
			gamestate_set(GS_WIN);
			return;
		}
		
		if (p_money == 0)
		{
			gamestate_set(GS_GAMEOVER);
			return;
		}
		
		manual_mail = 0;
		manual_scrape = 0;
		ui_update_required = 1;
		switch_screen(ui_menu, 0);
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
		switch_screen(MENU_MAIN, 1);
	}
	
	if (joy & J_A || joy & J_B)
	{
		select();
		handle_click();
	}
	
	if (ui_update_required != 0)
	{
		hud_update(2, p_money);
		hud_update(9, p_emails);
		hud_update(16, p_targets);
	}
}

