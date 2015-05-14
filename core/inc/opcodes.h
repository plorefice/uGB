//
//  opcodes.h
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#ifndef _CORE__OPCODES_H_
#define _CORE__OPCODES_H_

#include "types.h"

typedef struct {
    opcode opc;    ///< OPCode
    s8 disp;       ///< Optional signed displacement
    union {
        byte d8;   ///< 1-byte immediate
        word d16;  ///< 2-byte immediate
    } imm;         ///< Optional immediate data
} instr;

#endif /* _CORE__OPCODES_H_ */