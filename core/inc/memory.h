//
//  memory.h
//  GBEmu
//
//  Created by Pietro Lorefice on 14/05/15.
//
//

#ifndef _CORE__MEMORY_H_
#define _CORE__MEMORY_H_

#include "types.h"

typedef struct {
	byte * ROM_BNK_00;
	byte * ROM_BNK_NN;
	byte * VRAM;
	byte   ERAM [0x2000];
	byte   WRAM [0x2000];
	byte   OAM  [0xA0];
	byte   IOP  [0x80];
	byte   ZRAM [0x80];

	bool   booting;
} memory;

void memory_init(memory * mem);

void memory_w8(memory * mem, word addr, byte data);
void memory_w16(memory * mem, word addr, word data);
byte memory_r8(memory * mem, word addr);
word memory_r16(memory * mem, word addr);

#endif /* _CORE__MEMORY_H_ */
