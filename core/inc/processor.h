//
//  processor.h
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#ifndef _CORE__PROCESSOR_H_
#define _CORE__PROCESSOR_H_

#include "types.h"
#include "memory.h"
#include "cartridge.h"

typedef struct {
	memory MEM;                  ///< 64K memory map

    r16 AF, BC, DE, HL, SP, PC;  ///< 16-bit registers
	bit Z, N, H, C;              ///< flag bits

	bit hWFI;                    ///< halt until interrupt
	bit IRQE;                    ///< interrupt enabled
} processor;


void proc_init(processor * z80);
void proc_fetch(processor * z80);

void proc_load_cart(processor * z80, cartridge * cart);

void proc_clrflg(processor * z80);

void reg_loadH(r16 * reg, byte b);
void reg_loadL(r16 * reg, byte b);
void reg_loadHL(r16 * reg, word w);

#endif /* _CORE__PROCESSOR_H_ */
