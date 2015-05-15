//
//  processor.c
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#include "processor.h"
#include "opcodes.h"


void
proc_init(processor * z80)
{
	word addr;

	memory_init(&z80->MEM);

	z80->hWFI = 0;

	/* ----- BOOTSTRAP ----- */
	/* setup PC and SP */
	z80->PC = 0x100;
	z80->SP = 0xFFFE;

	/* zero VRAM content */
	//for (addr = 0x9FFF; addr >= 0x8000; --addr)
	//	memory_w8(&z80->MEM, addr, 0x00);

	/* TODO: setup audio */

	/* setup BG palette */
	//memory_w8(&z80->MEM, 0xFF47, 0xFC);

	/* TODO: load logo from cart */
	/* TODO: scroll logo*/
}

void
proc_fetch(processor * z80)
{
	if (!z80->hWFI)
	{
		opcode opc = memory_r8(&z80->MEM, z80->PC++);
		dispatch[opc](z80);
	}
}

void
proc_load_cart(processor * z80, cartridge * cart)
{
	z80->MEM.ROM_BNK_00 = &cart->ROM_BNK_00[0];
	z80->MEM.ROM_BNK_NN = &cart->ROM_BNK_01[0];
}

void 
proc_clrflg(processor * z80)
{
	z80->Z = 0;
	z80->H = 0;
	z80->N = 0;
	z80->C = 0;
}

void 
reg_loadH(r16 * reg, byte b)
{
	*reg = (b << 8) | (*reg & 0x00FF);
}

void
reg_loadL(r16 * reg, byte b)
{
	*reg = (*reg & 0xFF00) | b;
}

void 
reg_loadHL(r16 * reg, word w)
{
	*reg = w;
}