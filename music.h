
#ifndef _MUSIC_H
#define _MUSIC_H

void music_init();
void music_tick();

void music_play(UINT8* music, UINT16 len, UINT16 bassdrum_pattern, UINT16 snare_pattern, UINT16 hit_pattern);

#endif /* _MUSIC_H */