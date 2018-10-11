#include <gb/gb.h>
#include "sfx.h"

void sfx_type()
{
	NR10_REG = 0x21u;
	NR11_REG = 0x38u;
	NR12_REG = 0x30u;
	NR13_REG = 0xA4u;
	NR14_REG = 0xC0u;
}

void sfx_upgrade()
{
	NR10_REG = 0x51u;
	NR11_REG = 0x81u;
	NR12_REG = 0xF3u;
	NR13_REG = 0xA4u;
	NR14_REG = 0xC0u;
}

void sfx_fail()
{
	NR10_REG = 0x7Au;
	NR11_REG = 0x81u;
	NR12_REG = 0xF3u;
	NR13_REG = 0x00u;
	NR14_REG = 0xC1u;
}