//
//  memory.c
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#include "memory.h"


static void memory_write(memory * mem, word addr, byte data);
static byte memory_read(memory * mem, word addr);


void 
memory_init(memory * mem)
{

}

void
memory_w8(memory * mem, word addr, byte data)
{
	memory_write(mem, addr, data);
}

void
memory_w16(memory * mem, word addr, word data)
{
	memory_write(mem, addr, data & 0x00FF);
	memory_write(mem, addr + 1, data >> 8);
}

byte
memory_r8(memory * mem, word addr)
{
	return memory_read(mem, addr);
}

word
memory_r16(memory * mem, word addr)
{
	byte lsb = memory_read(mem, addr);
	byte msb = memory_read(mem, addr + 1);

	return ((msb << 8) | lsb);
}


static void 
memory_write(memory * mem, word addr, byte data)
{
	switch (addr & 0xF000)
	{
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		mem->ROM_BNK_00[addr] = data;

	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		mem->ROM_BNK_NN[addr] = data;

	case 0x8000:
	case 0x9000:
		mem->VRAM[addr & 0x1FFF] = data;

	case 0xA000:
	case 0xB000:
		mem->ERAM[addr & 0x1FFF] = data;

	case 0xC000:
	case 0xD000:
		mem->WRAM[addr & 0x1FFF] = data;

	case 0xE000:
		mem->WRAM[addr & 0x1FFF] = data;

	case 0xF000:
		switch (addr & 0x0F00)
		{
		case 0x0E00:
			mem->OAM[addr & 0x009F] = data;

		case 0x0F00:
			if ((addr & 0x00F0) < 0x0080)
				mem->IOP[addr & 0x007F] = data;
			else
				mem->ZRAM[addr & 0x007F] = data;

		default:
			mem->WRAM[addr & 0x1FFF] = data;
		}
	}
}

static byte
memory_read(memory * mem, word addr)
{
	switch (addr & 0xF000)
	{
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return mem->ROM_BNK_00[addr];
		
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return mem->ROM_BNK_NN[addr];

	case 0x8000:
	case 0x9000:
		return mem->VRAM[addr & 0x1FFF];

	case 0xA000:
	case 0xB000:
		return mem->ERAM[addr & 0x1FFF];

	case 0xC000:
	case 0xD000:
		return mem->WRAM[addr & 0x1FFF];

	case 0xE000:
		return mem->WRAM[addr & 0x1FFF];

	case 0xF000:
		switch (addr & 0x0F00)
		{
		case 0x0E00:
			return mem->OAM[addr & 0x009F];

		case 0x0F00:
			if ((addr & 0x00F0) < 0x0080)
				return mem->IOP[addr & 0x007F];
			else
				return mem->ZRAM[addr & 0x007F];

		default:
			return mem->WRAM[addr & 0x1FFF];
		}

	default:
		return 0x00; /* never happens! */
	}
}