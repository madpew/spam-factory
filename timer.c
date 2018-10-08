#include <gb/gb.h>
#include "timer.h"
#include "music.h"

UINT8 timer_count = 0U;
UINT8 timer_flag = 0U;

void timer_tick()
{
	timer_count++;

	if (timer_count % TIMERMOD_MUSIC == 0) music_tick();
	if (timer_count % TIMERMOD_ANIMATION == 0) timer_set(TIMERFLAG_ANIMATION);
	if (timer_count % TIMERMOD_ICONS == 0) timer_set(TIMERFLAG_ICONS);
	if (timer_count % TIMERMOD_GAMESTEP == 0) timer_set(TIMERFLAG_GAMESTEP);
}

void timer_init()
{
	TMA_REG = TIMERMOD;
	TAC_REG	= 4U;

	timer_flag = 0U;
	timer_count = 0U;

	disable_interrupts();
    add_TIM(timer_tick);
    enable_interrupts();
    set_interrupts(TIM_IFLAG | VBL_IFLAG);
}