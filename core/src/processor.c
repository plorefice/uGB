//
//  processor.c
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#include "processor.h"
#include "opcodes.h"

#include <stdio.h>


void
proc_init(processor * z80)
{
	z80->hWFI = 0;
	z80->IRQE = 0;
	z80->gClk = 0;

	z80->PC = 0x0;
}

void
proc_fetch(processor * z80)
{
	if (!z80->hWFI)
	{
		r16 prevPC = z80->PC;
		opcode opc = memory_r8(z80->MEM, z80->PC++);
		r16 afterPC = z80->PC;

		printf("%s\n", opcode_name(opc));

		dispatch[opc](z80);

		if (prevPC == 0xFE && afterPC == 0xFF)
			z80->MEM->booting = false;
	}
}

void
proc_load_cart(processor * z80, cartridge * cart)
{
	z80->MEM->ROM_BNK_00 = &cart->ROM_BNK_00[0];
	z80->MEM->ROM_BNK_NN = &cart->ROM_BNK_01[0];
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