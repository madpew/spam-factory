#include <gb/gb.h>
#include "music.h"

#define SILENCE 72

const UWORD frequencies[] = {
  44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986,
  1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
  1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
  1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
  1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
  1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015
};


UINT16 frequency;
UINT8 note;
UINT8 channel;
UINT16 drumMask;

// 
UINT16 music_index;
UINT16 music_step;

UINT8* note_data;
UINT16 note_data_length;
UINT16 bassdrum_pattern;
UINT16 snaredrum_pattern;
UINT16 hit_pattern;

void music_play(UINT8* music, UINT16 len, UINT16 bassdrum, UINT16 snare, UINT16 hit)
{
	note_data = music;
	note_data_length = len;
	bassdrum_pattern = bassdrum;
	snaredrum_pattern = snare;
	hit_pattern = hit;
	music_step = 0;
	music_index = 0;
}

void music_init()
{
	NR21_REG = 0x80U; // Wave Duty  00 1/8   40 1/4     80 1/2   
    NR22_REG = 0xB0U | 0x01U; // 0xF0 for full volume | sound length (bit 0-2)

	NR30_REG = 0x80U; // Master ON
	NR31_REG = 0xF5U; // SOUND LENGTH // F8
	NR32_REG = 0x20U; //NR32_REG = 0x40U; // half

	// WAV Ram
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
}

void music_tick()
{
	if (note_data == 0) return;
	
	if (music_index >= note_data_length)
	{
		music_index = 0;
		music_step = 0;
	}
	
	note = *(note_data + music_index);
	music_index++;
	channel = note & 0x80U;
	frequency = frequencies[note & ~0x80U];

	drumMask = 0x0001ul << (music_step & 0xFu);

	if ((drumMask & bassdrum_pattern) == drumMask)
	{
		//NR41_REG = 0x28u;
		NR41_REG = 0x30u; // 3F Max
		NR42_REG = 0xF1u;
		NR43_REG = 0x7Eu;
		NR44_REG = 0xC0u; // 80
	}
	else if ((drumMask & snaredrum_pattern) == drumMask)
	{
		NR41_REG = 0x3Fu;
		NR42_REG = 0xC1u;
		NR43_REG = 0x33u;
		NR44_REG = 0x80u;
	}
	else if ((drumMask & hit_pattern) == drumMask)
	{
		NR41_REG = 0x32u;
		NR42_REG = 0xA1u;
		NR43_REG = 0x00u;
		NR44_REG = 0xC0u;
	}

	music_step++;

	if (channel == 0)
	{
		//NR32_REG = 0;
		if (note == SILENCE) return;
		NR23_REG = 0xFFU & frequency;
		NR24_REG = 0x80U | (frequency >> 8);
	}
	else
	{
		// use 3 for second track
		NR31_REG = 0xB0u; //0x80U fullnote;
		NR32_REG = 0x40u;
		NR33_REG = frequency;
		NR34_REG = 0xC0u | (frequency >> 8); // 0x80

		note = *(note_data + music_index++);

		if (note == SILENCE) return;

		frequency = frequencies[note];
		NR23_REG = frequency;
		NR24_REG = 0x80u | (frequency >> 8);
	}
}