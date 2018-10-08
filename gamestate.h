#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#define GS_NONE 0
#define GS_PREROLL 1
#define GS_MENU 2
#define GS_INTRO 3
#define GS_PLAYING 4
#define GS_EMAIL 5
#define GS_OFFER 6
#define GS_INFO 7
#define GS_GAMEOVER 8
#define GS_WIN 9

extern UINT8 Gamestate;

void gamestate_set(UINT8 state);

void gamestate_tick();

#endif /* _GAMESTATE_H */