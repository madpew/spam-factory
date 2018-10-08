
#ifndef _TIMER_H
#define _TIMER_H

#define TIMERFLAG_ANIMATION	1U
#define TIMERFLAG_ICONS		2U
#define TIMERFLAG_GAMESTEP	4U

#define TIMERMOD	30u

#define TIMERMOD_MUSIC		4U
#define TIMERMOD_ANIMATION	4U 
#define TIMERMOD_ICONS		16U
#define TIMERMOD_GAMESTEP	16U


#define timer_set(mask) (timer_flag |= mask)
#define timer_clear(mask) (timer_flag &= ~mask)
#define timer_check(mask) ((timer_flag & mask) == mask)

void timer_init();
void timer_tick();

extern UINT8 timer_count;
extern UINT8 timer_flag;

#endif /* _TIMER_H */