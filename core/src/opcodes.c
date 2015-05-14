//
//  opcodes.c
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#include "opcodes.h"

#include "processor.h"

typedef void (*iexec)(processor *, instr *);


static void exec_0x00 (processor * z80, instr * ins)
{
    /* NOP */
    return;
}

static void exec_0x01 (processor * z80, instr * ins)
{
    /* LD BC, d16 */
    z80->BC = ins->imm.d16;
}

static void exec_0x02 (processor * z80, instr * ins)
{
    /* LD (BC), A */
    z80->MEM[z80->BC] = z80->AF;
    z80->BC = z80->BC + 1;
}


static const iexec exec[] = {
    exec_0x00,
    exec_0x01
};